#pragma once

#include "Stopwatch.h" // For convenience
#include <vector>
#include <numeric>
#include <cmath>
#include <limits>
#include <mutex>

namespace mini_ros {

// Simple thread-safe statistics tracker
class Statistics {
public:
    Statistics()
        : min_(std::numeric_limits<double>::max()),
          max_(std::numeric_limits<double>::min()),
          count_(0),
          sum_(0.0),
          sumSq_(0.0) {}

    void add(double value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (value < min_) min_ = value;
        if (value > max_) max_ = value;
        count_++;
        sum_ += value;
        sumSq_ += value * value;
    }

    double mean() const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (count_ == 0) return 0.0;
        return sum_ / count_;
    }

    double stddev() const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (count_ < 2) return 0.0;
        double mean = sum_ / count_;
        double variance = (sumSq_ - (sum_ * mean)) / (count_ - 1);
        return std::sqrt(variance);
    }
    
    double min() const { 
        std::lock_guard<std::mutex> lock(mutex_);
        return (count_ == 0) ? 0.0 : min_; 
    }
    
    double max() const { 
        std::lock_guard<std::mutex> lock(mutex_);
        return (count_ == 0) ? 0.0 : max_; 
    }
    
    size_t count() const { 
        std::lock_guard<std::mutex> lock(mutex_);
        return count_; 
    }

private:
    mutable std::mutex mutex_;
    double min_;
    double max_;
    size_t count_;
    double sum_;
    double sumSq_;
};

} // namespace mini_ros