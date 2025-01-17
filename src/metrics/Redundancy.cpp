#include "Redundancy.hpp"
#include <math.h>
#include <vector>
#include <base-logging/Logging.hpp>
#include "../reasoning/ResourceMatch.hpp"
#include "ModelSurvivability.hpp"
#include "../vocabularies/OM.hpp"

using namespace owlapi::model;
using namespace owlapi::vocabulary;

namespace moreorg {
namespace metrics {

Redundancy::Redundancy(const OrganizationModelAsk& organization,
        double defaultPoS,
        const owlapi::model::IRI& objectProperty)

    : Metric(REDUNDANCY, organization, objectProperty)
    , mDefaultProbabilityOfSurvival(defaultPoS)
{}

double Redundancy::computeSequential(const owlapi::model::IRIList& functions, const ModelPool& modelPool) const
{
    std::vector<double> metrics;
    for(const IRI& function : functions)
    {
        double metric = Metric::compute(function, modelPool);
        LOG_DEBUG_S << "Function: '" << function.toString() << "' --> metric: " << metric;
        metrics.push_back(metric);
    }

    // serial connection of all functions
    return serial(metrics);
}

double Redundancy::computeSequential(const std::vector<owlapi::model::IRISet>& functionalRequirement, const ModelPool& modelPool, bool sharedUse) const
{
    std::vector<double> metrics;
    for(const IRISet& functions : functionalRequirement)
    {
        double metric;
        if(sharedUse)
        {
            metric = Metric::computeSharedUse(functions, modelPool);
        } else {
            metric = Metric::computeExclusiveUse(functions, modelPool);
        }
        LOG_DEBUG_S << "Function: '" << functions << "' --> metric: " << metric;
        metrics.push_back(metric);
    }

    // serial connection of all functions
    return serial(metrics);
}

double Redundancy::computeMetric(const std::vector<OWLCardinalityRestriction::Ptr>& required, const std::vector<OWLCardinalityRestriction::Ptr>& available) const
{
    if(required.empty())
    {
        throw std::invalid_argument("moreorg::metrics::Redundancy: set of cardinality restriction to define requirements is empty");
    }

    // Strategies to compute redundancy:
    // 1. account for relevant functionality only
    //
    // Best common redundancy:
    //  --- account for full replacements
    //   >> assign minimal resources
    //   >>   only successful full assignment, remove these resources and try
    //   again with the rest
    //
    //   >> check how often that can be done -- compute redundancy for serial
    //   system, allow parallel system being
    //
    // Best redundancy: assign resource where it contributes the most, i.e.
    //  --- system parts with lowest level of survivability gets resource first
    //  (since they will most likely fail, that should be fair enough)

    // Firstly -- we need to find a proper match of required resources to
    // available resources, thus defining here a small Constraint Satisfaction
    // Problem already
    using namespace moreorg::reasoning;

    ModelBound::List modelBoundRemaining = ResourceMatch::toModelBoundList(available);
    ModelBound::List modelBoundRequired = ResourceMatch::toModelBoundList(required);

    if(!ResourceMatch::hasMinRequirements( modelBoundRequired ) )
    {
        throw std::invalid_argument("moreorg::metrics::Redundancy: model bound requires minimum requirements"
                "to complete redundancy computation, but none are provided");
    }

    LOG_DEBUG_S
        << "Available: " << ModelBound::toString(modelBoundRemaining)
        << std::endl
        << "Required: " << ModelBound::toString(modelBoundRequired);

    ResourceMatch::Solution solution;
    uint32_t fullModelRedundancy = 0;
    try {

        // Check how often a full redundancy of the top level model is given
        while(true)
        {
            solution = ResourceMatch::solve(modelBoundRequired, modelBoundRemaining, mOrganizationModelAsk);
            ++fullModelRedundancy;
            // Remove the consumed models from the list of available and try to
            // repeat solving
            // throws invalid_argument when model bounds are exceeded
            modelBoundRemaining = solution.substractMinFrom(modelBoundRemaining);
            LOG_DEBUG_S
                << "Solution: " << solution.toString()
                << std::endl
                << "Remaining: " << ModelBound::toString(modelBoundRemaining);
        }
    } catch(const std::exception& e)
    {
        LOG_DEBUG_S << "ResourceMatch failed: " << e.what();
    }

    LOG_INFO_S << "Full model redundancy count is at: " << fullModelRedundancy << std::endl
        << "   remaining: " << ModelBound::toString(modelBoundRemaining, 8);

    if(fullModelRedundancy == 0)
    {
        LOG_WARN_S << "Redundancy: the minimal resource requirements have not been fulfilled. Redundancy cannot be computed" << std::endl
            << "available: " << ModelBound::toString(modelBoundRemaining,4)
            << "required: " << ModelBound::toString(modelBoundRequired, 4);
        throw std::runtime_error("owlapi::metrics::Redundancy: minimal resource requirement have not been fulfilled");
    }

    //  Find weakest spot in the model
    //  Put all resources in that match
    //  iterate until no resources are left

    std::vector<ModelSurvivability> models;

    for(const OWLCardinalityRestriction::Ptr& cRestriction : required)
    {
        OWLObjectCardinalityRestriction::Ptr restriction =
            dynamic_pointer_cast<OWLObjectCardinalityRestriction>(cRestriction);
        if(!restriction)
        {
            throw
                std::runtime_error("moreorg::metrics::Redundancy::computeSequential:"
                        " expected OWLObjectCardinalityRestriction");
        }
        IRI qualification = restriction->getQualification();

        // Mean probability of failure
        // Probability of component failure
        // default is p=0.5
        double probabilityOfSurvival = 0;
        try {
            // Model should have an associated probability of failure if not
            // failure of parent component which be used (see punning strategy // in owlapi)
            OWLLiteral::Ptr value = mOrganizationModelAsk.ontology().getDataValue(qualification, vocabulary::OM::probabilityOfFailure());
            LOG_DEBUG_S << "Retrieved probability of failure for '" << qualification << ": " << value->getDouble();
            probabilityOfSurvival = 1 - value->getDouble();
        } catch(...)
        {
            LOG_DEBUG_S << "Using probability of failure for '" << qualification << ": 0.95";
            probabilityOfSurvival = mDefaultProbabilityOfSurvival;
        }

        ModelSurvivability survivability(restriction, probabilityOfSurvival, fullModelRedundancy);
        models.push_back(survivability);
    }

    // Best model fit: redundancy
    bool updated = false;
    do
    {
        updated = false;
        // Sort based on probability of survival -- try to maximize redundancy
        std::sort(models.begin(), models.end(), [](const ModelSurvivability& a, const ModelSurvivability& b)
                {
                    return a.getProbabilityOfSurvival() < b.getProbabilityOfSurvival();
                });

        ModelBound::List::iterator rit = modelBoundRemaining.begin();
        for(; rit != modelBoundRemaining.end();++rit)
        {
            ModelBound& remaining = *rit;
            bool hasPossibleMatch = false;

            // Try to fit remaining resources
            for(ModelSurvivability& survivability : models)
            {
                // Check if model can be used to strengthen the survivability
                if( survivability.getQualification() == remaining.model ||
                        mOrganizationModelAsk.ontology().isSubClassOf(remaining.model, survivability.getQualification()) )
                {
                    hasPossibleMatch = true;
                    try {
                        remaining.decrement();
                        survivability.increment();
                        updated = true;
                    } catch(...)
                    {
                        modelBoundRemaining.erase(rit);
                        --rit;
                        break;
                    }
                }
            }
            if(!hasPossibleMatch)
            {
                modelBoundRemaining.erase(rit);
                --rit;
            }
        }
    } while(updated);

    // Serial model of all subcomponents --> the full system
    double fullModelSurvival = 1;

    for(const ModelSurvivability& survivability : models)
    {
        LOG_INFO_S << "Probability of survival: " << survivability.toString();
        fullModelSurvival *= survivability.getProbabilityOfSurvival();
    }

    return fullModelSurvival;
}

double Redundancy::parallel(const std::vector<double>& probabilities)
{
    double probability = 1.0;
    for(const double& p : probabilities)
    {
        probability *= (1 - p);
    }

    return 1 - probability;

}

double Redundancy::serial(const std::vector<double>& probabilities)
{
    double probability = 1.0;
    for(const double& p : probabilities)
    {
        probability *= p;
    }
    return probability;
}

} // end namespace metrics
} // end namespace moreorg


