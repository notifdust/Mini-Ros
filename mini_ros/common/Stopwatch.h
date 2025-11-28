#pragma once

#include <chrono>

namespace mini_ros {

class Stopwatch {
public:
    using Clock = std::chrono::high_resolution_clock;

    Stopwatch() : startTime_(Clock::now()) {}

    void reset() {
        startTime_ = Clock::now();
    }

    // Returns elapsed time in seconds
    double elapsed() const {
        auto endTime = Clock::now();
        std::chrono::duration<double> diff = endTime - startTime_;
        return diff.count();
    }
    
    // Returns elapsed time in milliseconds
    double elapsed_ms() const {
        auto endTime = Clock::now();
        std::chrono::duration<double, std::milli> diff = endTime - startTime_;
        return diff.count();
    }

private:
    std::chrono::time_point<Clock> startTime_;
};

} // namespace mini_ros