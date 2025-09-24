#include <RTypeSrv/Version.hpp>

[[nodiscard]] RTYPE_SRV_API const char *rtype::srv::version() noexcept {
    return RTYPE_SRV_VER;
}

[[nodiscard]] RTYPE_SRV_API std::string rtype::srv::full_version() noexcept {
    return "R-Type server lib Ver. " RTYPE_SRV_VER;
}
