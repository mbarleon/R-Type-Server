#include <RTypeEcsVersion.hpp>
#include <RTypeNetworkVersion.hpp>
#include <iostream>

int main()
{
    std::cout << "ECS ver. " << rtype_ecs_version() << std::endl;
    std::cout << rtype_full_network_version() << std::endl;
    return 0;
}
