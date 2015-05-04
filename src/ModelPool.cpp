#include "ModelPool.hpp"
#include <sstream>

namespace organization_model {

ModelPoolDelta::ModelPoolDelta()
{}

ModelPoolDelta::ModelPoolDelta(const ModelPool& pool)
{
    ModelPool::const_iterator cit = pool.begin();
    for(; cit != pool.end(); ++cit)
    {
        insert(std::pair<owlapi::model::IRI, int>(cit->first, cit->second));
    }
}

bool ModelPoolDelta::isNegative() const
{
    ModelPoolDelta::const_iterator cit = this->begin();
    for(; cit != this->end(); ++cit)
    {
        if(cit->second < 0)
        {
            return true;
        }
    }
    return false;
}

ModelPool ModelPoolDelta::toModelPool() const
{
    ModelPool modelPool;
    ModelPoolDelta::const_iterator cit = this->begin();
    for(; cit != this->end(); ++cit)
    {
        if(cit->second < 0)
        {
            throw std::runtime_error("organization_model::ModelPoolDelta contains negative values -- cannot convert to ModelPool");
        } else if(cit->second > 0)
        {
            modelPool[cit->first] = cit->second;
        }
    }
    return modelPool;
}

std::string ModelPoolDelta::toString() const
{
    std::stringstream ss;
    ModelPoolDelta::const_iterator cit = this->begin();
    ss << "ModelPool:" << std::endl;
    for(; cit != this->end(); ++cit)
    {
        ss << "    " << cit->first << " : " << cit->second << std::endl;
    }
    return ss.str();
}

} // end namespace organization_model