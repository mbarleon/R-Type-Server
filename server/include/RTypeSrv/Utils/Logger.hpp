#include <RTypeSrv/Utils/Singleton.hpp>
#include <iostream>
#include <mutex>
#include <utility>

namespace rtype::srv::utils {

class Logger : public Singleton<Logger>
{
        friend class Singleton;

    public:
        template<typename... Args>
        void cout(Args &&...args)
        {
            std::lock_guard<std::mutex> lock(_mtx);
            (std::cout << ... << args) << std::endl;
        }

        template<typename... Args>
        void cerr(Args &&...args)
        {
            std::lock_guard<std::mutex> lock(_mtx);
            (std::cerr << ... << args) << std::endl;
        }

    protected:
        constexpr explicit Logger() = default;
        ~Logger() noexcept = default;

    private:
        std::mutex _mtx;
};

template<typename... Args>
static inline void cout(Args &&...args)
{
    Logger::getInstance().cout(std::forward<Args>(args)...);
}

template<typename... Args>
static inline void cerr(Args &&...args)
{
    Logger::getInstance().cerr(std::forward<Args>(args)...);
}

}// namespace rtype::srv::utils
