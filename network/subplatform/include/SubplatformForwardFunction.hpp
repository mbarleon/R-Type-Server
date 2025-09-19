#pragma once

// clang-format off
#include <NetworkApi.hpp>
namespace rtype::network::subplatform {}

#define RTYPE_NET_FWD_FUNC(NAME, ...) RTYPE_NET_API decltype(rtype::network::subplatform::NAME()) rtype::network::NAME() __VA_OPT__(__VA_ARGS__) { return subplatform::NAME(); }
// clang-format on
