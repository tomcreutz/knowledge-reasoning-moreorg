#include "Agent.hpp"

namespace organization_model {

Agent::Agent()
{}

Agent::Agent(const AtomicAgent& atomicAgent)
{
    mAtomicAgents.insert(atomicAgent);
}

Agent::Agent(const AtomicAgent::List& atomicAgents)
    : mAtomicAgents(atomicAgents.begin(), atomicAgents.end())
{
}

Agent::Agent(const AtomicAgent::Set& atomicAgents)
    : mAtomicAgents(atomicAgents)
{
}

facades::Robot Agent::getFacade(const OrganizationModelAsk& ask) const
{
    ModelPool modelPool = getType();
    return facades::Robot::getInstance(modelPool, ask);
}

void Agent::update(const OrganizationModelAsk& ask)
{
    mEnergyProviderPolicy.update(*this, ask);
}

ModelPool Agent::getType() const
{
    ModelPool pool;
    for(const AtomicAgent& agent : mAtomicAgents)
    {
        pool[agent.getModel()] += 1;
    }
    return pool;
}

std::string Agent::toString(size_t indent) const
{
    std::string hspace(indent,' ');
    std::stringstream ss;
    ss << hspace << "Agent:" << std::endl;
    for(const AtomicAgent& aa : mAtomicAgents)
    {
        ss << hspace << "    " << aa.toString() << std::endl;
    }
    return ss.str();
}

AtomicAgent::Set Agent::allAtomicAgents(const Agent::Set& agents)
{
    AtomicAgent::Set atomicAgents;
    for(const Agent& agent : agents)
    {
        atomicAgents.insert(agent.getAtomicAgents().begin(),
                agent.getAtomicAgents().end());
    }
    return atomicAgents;
}

AtomicAgent::List Agent::getIntersection(const Agent& other) const
{
    return AtomicAgent::getIntersection(mAtomicAgents, other.mAtomicAgents);
}

bool Agent::intersectsWith(const Agent& other) const
{
    return !AtomicAgent::getIntersection(mAtomicAgents, other.mAtomicAgents).empty();
}

AtomicAgent::List Agent::getDifference(const Agent& other) const
{
    return AtomicAgent::getDifference(mAtomicAgents, other.mAtomicAgents);
}

}
