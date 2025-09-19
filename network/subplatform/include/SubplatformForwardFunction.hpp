#pragma once

// clang-format off
#define RTYPE_NET_FWD_FUNC(NAME, ...) namespace rtype::network { RTYPE_NET_API decltype(subplatform::NAME()) NAME() __VA_OPT__(__VA_ARGS__) { return subplatform::NAME(); } }
// clang-format on
