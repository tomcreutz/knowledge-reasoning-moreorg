#include "QueryCache.hpp"

namespace organization_model {

std::pair<owlapi::model::OWLCardinalityRestriction::PtrList, bool> QueryCache::getCachedResult(const ModelPool& modelPool,
        const owlapi::model::IRI& objectProperty,
        owlapi::model::OWLCardinalityRestriction::OperationType operationType,
        bool max2Min) const
{
    using namespace owlapi::model;
    std::pair<owlapi::model::OWLCardinalityRestriction::PtrList, bool> result;
    CRQuery query = std::make_tuple(modelPool, objectProperty, operationType, max2Min);
    CRQueryResults::const_iterator cit = mQueryResults.find(query);
    if(cit == mQueryResults.end())
    {
        result.second = false;
    } else {
        result.first = cit->second;
        result.second = true;
    }
    return result;
}

void QueryCache::cacheResult(const ModelPool& modelPool,
        const owlapi::model::IRI& objectProperty,
        owlapi::model::OWLCardinalityRestriction::OperationType operationType,
        bool max2Min,
        const owlapi::model::OWLCardinalityRestriction::PtrList& list)
{
    CRQuery query = std::make_tuple(modelPool, objectProperty, operationType, max2Min);
    mQueryResults.emplace(query, list);
}

} // end namespace organization_model

