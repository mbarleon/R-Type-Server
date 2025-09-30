#pragma once

#include <RTypeSrv/Api.hpp>
#include <exception>
#include <sstream>
#include <string>
#include <utility>

namespace rtype::srv {

#ifdef _WIN32
    #pragma warning(push)
    #pragma warning(disable : 4275)
    #pragma warning(disable : 4251)
#endif

class RTYPE_SRV_API Exception final : public std::exception
{
    public:
        template<typename... Args>
        constexpr explicit Exception(std::string where, Args &&...args)
            : _where(std::move(where)), _what(concatStrings(std::forward<Args>(args)...))
        {
        }
        ~Exception() noexcept override = default;

        [[nodiscard]] const char *what() const noexcept override;
        [[nodiscard]] const char *where() const noexcept;

    private:
        const std::string _where;
        const std::string _what;

        template<typename... Args>
        static std::string concatStrings(Args &&...args)
        {
            std::ostringstream oss;
            if constexpr (sizeof...(args) > 0) {
                (oss << ... << args);
            }
            return oss.str();
        }
};

#ifdef _WIN32
    #pragma warning(pop)
#endif

}// namespace rtype::srv
