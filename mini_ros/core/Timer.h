#pragma once

#include <chrono>
#include <functional>
#include "Statistics.h"

namespace mini_ros {

class Timer {
public:
    using CallbackT = std::function<void()>;
    using Clock = std::chrono::high_resolution_clock;

    Timer(std::chrono::duration<double> period, CallbackT callback)
        : period_(period), callback_(callback), nextRunTime_(Clock::now() + period) {}

    void spinOnce() {
        auto now = Clock::now();
        if (now >= nextRunTime_) {
            Stopwatch sw;
            callback_();
            stats_.add(sw.elapsed());

            // Schedule next run
            nextRunTime_ += period_;
            // Handle timer overruns (if processing took longer than period)
            if (nextRunNime_ < now) {
                nextRunTime_ = now + period_;
            }
        }
    }
    
    Statistics getStats() const { return stats_; }

private:
    std::chrono::duration<double> period_;
    CallbackT callback_;
    std::chrono::time_point<Clock> nextRunTime_;
    Statistics stats_;
};

} // namespace mini_ros