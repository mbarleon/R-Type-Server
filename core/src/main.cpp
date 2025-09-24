#include <RTypeEcsVersion.hpp>
#include <RTypeNet/Version.hpp>
#include <iostream>

int main()
{
    std::cout << "ECS ver. " << rtype_ecs_version() << std::endl;
    std::cout << rtype::network::full_version() << std::endl;
    return 0;
}
