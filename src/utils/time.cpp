#include <string.h>

#include "utils/time.h"

utils::time::Timer::Timer()
{
}

utils::time::Timer::~Timer()
{
}

void utils::time::Timer::start(void)
{
    this->_start_time = std::chrono::high_resolution_clock::now();
}

void utils::time::Timer::stop(void)
{
    this->_stop_time = std::chrono::high_resolution_clock::now();
}

std::chrono::nanoseconds utils::time::Timer::elapsed_time(void) const
{
    return (this->_stop_time - this->_start_time);
}

std::string utils::time::timestamp(void)
{
    auto now = std::chrono::high_resolution_clock::now();
    std::time_t now_c = std::chrono::high_resolution_clock::to_time_t(now);
    char mbstr[100] = {0};
    if (std::strftime(mbstr, sizeof(mbstr) - 1, "%Y-%m-%d %H:%M:%S %Z", std::localtime(&now_c)))
    {
        return std::string(mbstr);
    }
    else
    {
        return std::string("");
    }
}

