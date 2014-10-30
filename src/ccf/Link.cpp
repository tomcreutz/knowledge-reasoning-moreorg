#include "Link.hpp"
#include <boost/foreach.hpp>

namespace multiagent {
namespace ccf {

Link::Link() {}

Link::Link(Interface first, Interface second)
{
    if(first < second)
    {
        mFirst = first;
        mSecond = second;
    } else {
        mSecond = first;
        mFirst = second;
    }
} 

LinkGroup Link::getGroup() const 
{    
    return LinkGroup(mFirst.getActor(), mSecond.getActor());
}

LinkType Link::getType() const
{
    return LinkType(mFirst.getType(), mSecond.getType());
}

bool Link::operator<(const Link& other) const
{
    if(other.mFirst < mFirst)
    {
        return true;
    } else if(other.mFirst == mFirst)
    {
        return other.mSecond < mSecond;
    } else {
        return false;
    }
}

std::ostream& operator<<(std::ostream& os, const Link& link)
{
    os << link.getFirstInterface() << " -- " << link.getSecondInterface();
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Link>& links)
{
    BOOST_FOREACH(const Link& link, links)
    {
        os << link;
        os << ",";
    }
    return os;
}

} // end namespace ccf
} // end namespace multiagent