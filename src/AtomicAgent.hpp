#ifndef ORGANIZATION_MODEL_AGENT_INSTANCE_HPP
#define ORGANIZATION_MODEL_AGENT_INSTANCE_HPP

#include <owlapi/model/IRI.hpp>
#include <organization_model/ModelPool.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/set.hpp>
#include "OrganizationModelAsk.hpp"
#include "facets/Robot.hpp"

namespace organization_model {

/**
 * \class AtomicAgent
 * \brief An individual unsplittable agent
 */
class AtomicAgent
{
    friend class boost::serialization::access;
    /// type of model this role fulfills
    owlapi::model::IRI mModel;
    /// id or the role
    size_t mId;
    /// Name of the instance derived from model IRI and id
    std::string mName;
public:
    typedef std::vector<AtomicAgent> List;
    typedef std::set<AtomicAgent> Set;

    /**
     * Default constructor to allow usage in lists and maps
     */
    AtomicAgent();

    /**
     * Preferred role constructor
     * \param name Name/Id of the role
     * \param model Model identification
     */
    AtomicAgent(size_t id, const owlapi::model::IRI& model);

    const owlapi::model::IRI& getModel() const { return mModel; }
    size_t getId() const  { return mId; }
    const std::string& getName() const { return mName; }

    std::string toString() const;
    static std::string toString(const List& agents, size_t indent = 0);
    static std::string toString(const Set& agents, size_t indent = 0);
    static organization_model::ModelPool getModelPool(const List& agents);
    static organization_model::ModelPool getModelPool(const Set& agents);

    bool operator<(const AtomicAgent& other) const;
    bool operator==(const AtomicAgent& other) const { return mModel == other.mModel && mId == other.mId; }
    bool operator!=(const AtomicAgent& other) const { return ! (*this == other); }

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & mModel;
        ar & mId;
        ar & mName;
    }

    /**
     * Creates the list of agents, that can be created from a given model pool,
     * assuming that it contains only types as keys
     */
    static List createAtomicAgents(const organization_model::ModelPool& modelPool);

    /**
     * Create the robot facet
     */
    facets::Robot getFacet(const OrganizationModelAsk& ask) const;
};

} // end namespace organization_model
#endif // ORGANIZATION_MODEL_AGENT_INSTANCE_HPP
