#pragma once

#include <RTypeSrv/Utils/NonCopyable.hpp>

namespace rtype::srv::utils {

template<class C>
class RTYPE_SRV_API Singleton : public NonCopyable
{
    public:
        static C &getInstance()
        {
            static C instance;
            return instance;
        }

    protected:
        constexpr explicit Singleton() = default;
        ~Singleton() noexcept = default;
};

}// namespace rtype::srv::utils
