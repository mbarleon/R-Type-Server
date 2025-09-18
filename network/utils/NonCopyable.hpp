#pragma once

#include "NetworkApi.hpp"

namespace rtype::network::utils {

class RTYPE_NET_HIDE_FROM_ABI NonCopyable
{
    public:
        NonCopyable(const NonCopyable &other) = delete;
        NonCopyable &operator=(const NonCopyable &rhs) = delete;
        NonCopyable(NonCopyable &&other) = delete;
        NonCopyable &operator=(NonCopyable &&rhs) = delete;

    protected:
        constexpr explicit NonCopyable() = default;
        ~NonCopyable() noexcept = default;
};

}// namespace rtype::network::utils
