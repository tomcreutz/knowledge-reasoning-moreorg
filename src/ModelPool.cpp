#include "ModelPool.hpp"
#include <sstream>

namespace organization_model {

ModelPool::ModelPool()
    : std::map<owlapi::model::IRI, size_t>()
{}

ModelPool::ModelPool(const ModelCombination& modelCombination)
    : std::map<owlapi::model::IRI, size_t>()
{
    ModelCombination::const_iterator cit = modelCombination.begin();
    for(; cit != modelCombination.end(); ++cit)
    {
        const owlapi::model::IRI& model = *cit;
        (*this)[model] += 1;
    }
}

void ModelPool::setResourceCount(const owlapi::model::IRI& resource, size_t count)
{
    (*this)[resource] = count;
}

std::string ModelPool::toString() const
{
    std::stringstream ss;
    ModelPool::const_iterator cit = this->begin();
    ss << "ModelPool:" << std::endl;
    for(; cit != this->end(); ++cit)
    {
        ss << "    " << cit->first << " : " << cit->second << std::endl;
    }
    return ss.str();
}


ModelPool ModelPool::applyUpperBound(const ModelPool& upperBounds) const
{
    ModelPool modelPool;

    ModelPool::const_iterator cit = this->begin();
    for(; cit != this->end(); ++cit)
    {
        const owlapi::model::IRI& model = cit->first;
        ModelPool::const_iterator bit = upperBounds.find(model);
        if(bit != upperBounds.end())
        {
            modelPool[model] = std::min(cit->second, bit->second);
        }
    }
    return modelPool;
}

ModelCombination ModelPool::toModelCombination() const
{
    ModelCombination combination;
    ModelPool::const_iterator cit = this->begin();
    for(; cit != this->end(); ++cit)
    {
        for(size_t i = 0; i < cit->second; ++i)
        {
            combination.push_back(cit->first);
        }
    }
    return combination;
}

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
    ss << "ModelPoolDelta:" << std::endl;
    for(; cit != this->end(); ++cit)
    {
        ss << "    " << cit->first << " : " << cit->second << std::endl;
    }
    return ss.str();
}

std::vector<owlapi::model::IRI> ModelPoolDelta::getModels(const ModelPool& modelPool)
{
    std::vector<owlapi::model::IRI> models;
    ModelPool::const_iterator cit  = modelPool.begin();
    for(; cit != modelPool.end(); ++cit)
    {
        models.push_back(cit->first);
    }
    return models;
}

} // end namespace organization_model
