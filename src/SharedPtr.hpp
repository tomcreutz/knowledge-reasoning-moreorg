#ifndef ORGANIZATION_MODEL_SHARED_PTR_HPP
#define ORGANIZATION_MODEL_SHARED_PTR_HPP

#if __cplusplus <= 199711L
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#else
#include <memory>
#include <functional>
#endif

namespace organization_model
{
#if __cplusplus <= 199711L
    using ::boost::shared_ptr;
    using ::boost::make_shared;
    using ::boost::dynamic_pointer_cast;
    using ::boost::static_pointer_cast;
    using ::boost::function1;
#else
    using ::std::shared_ptr;
    using ::std::make_shared;
    using ::std::dynamic_pointer_cast;
    using ::std::static_pointer_cast;
    template <class T, class U>
    using function1 = ::std::function<T(U)>;
#endif
}

#endif // ORGANIZATION_MODEL_SHARED_PTR_HPP
