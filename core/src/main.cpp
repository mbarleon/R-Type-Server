#include "CoreVersion.hpp"

#include <NetworkVersion.hpp>
#include <RTypeEcsVersion.hpp>
#include <iostream>

int main()
{
    std::cout << "ECS ver. " << rtype_ecs_version() << std::endl;
    std::cout << rtype::network::rtype_full_network_version() << std::endl;
    std::cout << rtype::network::rtype_network_core_version() << std::endl;
    return 0;
}
