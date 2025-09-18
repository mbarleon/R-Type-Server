#pragma once

#include "NonCopyable.hpp"

namespace rtype::network::utils {

template<class C>
class RTYPE_NET_HIDE_FROM_ABI Singleton : public NonCopyable
{
    public:
        C &getInstance()
        {
            static C instance;
            return instance;
        }

    protected:
        constexpr explicit Singleton() = default;
        ~Singleton() noexcept = default;
};

}// namespace rtype::network::utils
