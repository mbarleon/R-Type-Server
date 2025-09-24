#pragma once

#include <RTypeNet/Interfaces.hpp>
#include <RTypeSrv/Api.hpp>
#include <RTypeSrv/Utils/Singleton.hpp>
#include <thread>

namespace rtype::srv {

class RTYPE_SRV_API Server final : public utils::Singleton<Server>
{
        friend class utils::Singleton<Server>;

    public:
        void stopServer();
        void startServer();
        void initServer(const network::Endpoint &tcp_endpoint, std::size_t n_cores);

    protected:
        constexpr explicit Server() = default;
        constexpr ~Server() noexcept = default;

    private:
        bool _is_init = false;
        std::size_t _n_cores{};
        network::Endpoint _tcp_endpoint{};
};

}// namespace rtype::srv
