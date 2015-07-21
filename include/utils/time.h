#ifndef __OCTO_UTILS_TIME_H__
#define __OCTO_UTILS_TIME_H__

#include <chrono>
#include <string>

namespace utils
{

namespace time
{

class Timer
{
    public:
        Timer();
        ~Timer();

        void start(void);
        void stop(void);

        std::chrono::nanoseconds elapsed_time(void) const;

    protected:
        std::chrono::time_point<std::chrono::high_resolution_clock> _start_time;
        std::chrono::time_point<std::chrono::high_resolution_clock> _stop_time;
};

std::string timestamp(void);

}   // namespace time

}   // namespace utils

#endif

