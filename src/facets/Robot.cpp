#include "Robot.hpp"
#include <organization_model/vocabularies/OM.hpp>
#include <organization_model/vocabularies/Robot.hpp>
#include <base-logging/Logging.hpp>

using namespace owlapi::model;


namespace organization_model {
namespace facets {

std::map<ModelPool, Robot> Robot::msRobots;

const Robot& Robot::getInstance(const owlapi::model::IRI& actorModel, const OrganizationModelAsk& organizationModelAsk)
{
    ModelPool modelPool;
    modelPool[actorModel] = 1;
    return getInstance(modelPool, organizationModelAsk);
}

const Robot& Robot::getInstance(const ModelPool& modelPool, const OrganizationModelAsk& organizationModelAsk)
{
    std::map<ModelPool, Robot>::const_iterator cit = msRobots.find(modelPool);
    if(cit != msRobots.end())
    {
        return cit->second;
    } else {
        Robot robot(modelPool, organizationModelAsk);
        msRobots[modelPool] = robot;
        return msRobots[modelPool];
    }
}

Robot::Robot()
    : Facet()
{}

Robot::Robot(const owlapi::model::IRI& actorModel, const OrganizationModelAsk& organizationModelAsk)
    : Facet(organizationModelAsk)
    , mMinAcceleration(0.0)
    , mMaxAcceleration(0.0)
    , mNominalAcceleration(0.0)
    , mMinVelocity(0.0)
    , mMaxVelocity(0.0)
    , mNominalVelocity(0.0)
    , mMass(0.0)
    , mSupplyVoltage(0.0)
    , mEnergy(0.0)
    , mEnergyCapacity(0.0)
    , mNominalPowerConsumption(0.0)
    , mTransportCapacity(0)
    , mTransportDemand(0)
{

    mModelPool[actorModel] = 1;

    try {
        mNominalPowerConsumption = organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::nominalPowerConsumption())->getDouble();
        mMass = organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::mass())->getDouble();
        mSupplyVoltage = organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::supplyVoltage())->getDouble();
        mEnergyCapacity = organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::energyCapacity())->getDouble();
    } catch(const std::exception& e)
    {
        LOG_WARN_S << e.what();

        throw std::runtime_error("organization_model::facets::Robot loading of model '" + actorModel.toString() + "' failed: " + e.what());
    }

    // optional base mobility
    try {
        mMinAcceleration = organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::minAcceleration())->getDouble();
        mMaxAcceleration = organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::maxAcceleration())->getDouble();
        mNominalAcceleration = organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::nominalAcceleration())->getDouble();


        mMinVelocity = organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::minVelocity())->getDouble();
        mMaxVelocity = organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::maxVelocity())->getDouble();
        mNominalVelocity = organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::nominalVelocity())->getDouble();
    } catch(const std::exception& e)
    {
        LOG_INFO_S << e.what();
    }


    // optional transport capability
    try {
        mTransportDemand = organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::transportDemand())->getDouble();
        // get cardinality constraint
        mTransportCapacity = organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::transportCapacity())->getDouble();


    } catch(const std::exception& e)
    {
        LOG_INFO_S << e.what();
    }
}

Robot::Robot(const ModelPool& modelPool, const OrganizationModelAsk& organizationModelAsk)
    : Facet(organizationModelAsk)
    , mModelPool(modelPool)
    , mMinAcceleration(0.0)
    , mMaxAcceleration(0.0)
    , mNominalAcceleration(0.0)
    , mMinVelocity(0.0)
    , mMaxVelocity(0.0)
    , mNominalVelocity(0.0)
    , mMass(0.0)
    , mSupplyVoltage(0.0)
    , mEnergy(0.0)
    , mEnergyCapacity(0.0)
    , mNominalPowerConsumption(0.0)
    , mTransportCapacity(0)
    , mTransportDemand(0)
{

    for(const ModelPool::value_type& pair : mModelPool)
    {
        const owlapi::model::IRI& actorModel = pair.first;
        size_t modelCount = pair.second;
        try {
            mNominalPowerConsumption += modelCount*organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::nominalPowerConsumption())->getDouble();
            mMass += modelCount*organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::mass())->getDouble();
            mSupplyVoltage = organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::supplyVoltage())->getDouble();
            mEnergyCapacity = modelCount*organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::energyCapacity())->getDouble();
        } catch(const std::exception& e)
        {
            LOG_WARN_S << e.what();

            throw std::runtime_error("organization_model::facets::Robot loading of model '" + actorModel.toString() + "' failed: " + e.what());
        }

        // optional base mobility
        try {
            mMinAcceleration = std::max(mMinAcceleration, organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::minAcceleration())->getDouble());
            mMaxAcceleration = std::max(mMaxAcceleration, organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::maxAcceleration())->getDouble());
            mNominalAcceleration = std::max(mNominalAcceleration, organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::nominalAcceleration())->getDouble());

            mMinVelocity = std::max(mMinVelocity, organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::minVelocity())->getDouble());
            mMaxVelocity = std::max(mMaxVelocity, organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::maxVelocity())->getDouble());
            mNominalVelocity = std::max(mNominalVelocity, organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::nominalVelocity())->getDouble());
        } catch(const std::exception& e)
        {
            LOG_INFO_S << e.what();
        }

        // optional transport capability
        try {
            mTransportDemand += modelCount*organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::transportDemand())->getDouble();
            // get cardinality constraint
            mTransportCapacity += modelCount*organizationAsk().ontology().getDataValue(actorModel, vocabulary::Robot::transportCapacity())->getDouble();

        } catch(const std::exception& e)
        {
            LOG_INFO_S << e.what();
        }
    }

    // Assume there is only a single system transporting
    mTransportCapacity = mTransportCapacity - mModelPool.numberOfInstances() + 1;
}


uint32_t Robot::getTransportCapacity(const owlapi::model::IRI& model) const
{
    uint32_t capacity = 0;
    if(mModelPool.isAtomic())
    {
        const owlapi::model::IRI& actorModel = mModelPool.getAtomic();
        std::vector<OWLCardinalityRestriction::Ptr> restrictions = organizationAsk().ontology().getCardinalityRestrictionsForTarget(actorModel, vocabulary::OM::hasTransportCapacity(), model);
        capacity = getTransportCapacity();
        for(const OWLCardinalityRestriction::Ptr& r : restrictions)
        {
            switch(r->getCardinalityRestrictionType())
            {
                case OWLCardinalityRestriction::MAX:
                    capacity = std::min(capacity, r->getCardinality());
                    break;
                case OWLCardinalityRestriction::MIN:
                case OWLCardinalityRestriction::EXACT:
                case OWLCardinalityRestriction::UNKNOWN:
                    throw std::runtime_error("organization_model::facets::Robot::getTransportCapacity: expected max cardinality restriction, but got '" + OWLCardinalityRestriction::CardinalityRestrictionTypeTxt[r->getCardinalityRestrictionType()] + "'");
            }
        }
    } else {
        throw std::runtime_error("facets::Robot::getTransportCapacity: not implemented for composite systems");

    }
    return capacity;
}

int32_t Robot::getTransportSupplyDemand() const
{
    if(isMobile())
    {
        return getTransportCapacity();
    } else {
        return getTransportDemand();
    }
}

double Robot::estimatedEnergyCostFromTime(double timeInS) const
{
    // Watt * s
    // 1 Wattsekunde = 1/3600 Wh => Wh = Ws*3600
    //
    return (mNominalPowerConsumption*timeInS);
}

double Robot::estimatedEnergyCost(double distanceInM) const
{
    // Watt * m/ (m/s)
    // 1 Wattsekunde = 1/3600 Wh => Wh = Ws*3600
    //
    return estimatedEnergyCostFromTime( distanceInM/mNominalVelocity);
}

double Robot::estimatedRelativeEnergyCost(double distanceInM) const
{
    return estimatedEnergyCost(distanceInM) / mEnergyCapacity;
}

std::string Robot::toString(size_t indent) const
{
    std::string hspace(indent,' ');
    std::stringstream ss;
    ss << hspace << "Robot: " << mModelPool.toString(indent + 4) << std::endl;
    ss << hspace << "    mass (kg):                     " << mMass << std::endl;
    ss << hspace << "    supply voltage (V):            " << mSupplyVoltage << std::endl;
    ss << hspace << "    energy capacity (Wh):          " << mEnergyCapacity << std::endl;
    ss << hspace << "    nominal power consumption (W): " << mNominalPowerConsumption << std::endl;
    ss << hspace << "    min accelleration (m/s):       " << mMinAcceleration << std::endl;
    ss << hspace << "    max accelleration (m/s):       " << mMaxAcceleration << std::endl;
    ss << hspace << "    nominal accelleration (m/s):   " << mNominalAcceleration << std::endl;
    ss << hspace << "    min velocity (m/s):            " << mMinVelocity << std::endl;
    ss << hspace << "    max velocity (m/s):            " << mMaxVelocity << std::endl;
    ss << hspace << "    nominal velocity (m/s):        " << mNominalVelocity << std::endl;
    ss << hspace << "    transport demand (units):      " << mTransportDemand << std::endl;
    ss << hspace << "    transport capacity (units):    " << mTransportCapacity << std::endl;
    return ss.str();
}

bool Robot::isMobile() const
{
    owlapi::model::IRI moveTo = vocabulary::OM::resolve("MoveTo");
    bool supporting = organizationAsk().isSupporting(mModelPool, moveTo);
    return supporting;
}


} // end namespace facets
} // end namespace organization_model
