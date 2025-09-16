#include <iostream>
#include <RTypeEcsVersion.hpp>
#include <RTypeNetworkVersion.hpp>

int main(void)
{
    char *net_ver = rtype_full_network_version();

    std::cout << "ECS ver. " << rtype_ecs_version() << std::endl;
    std::cout << net_ver << std::endl;
    std::free(net_ver);
    return 0;
}
