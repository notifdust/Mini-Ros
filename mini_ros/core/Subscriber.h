#pragma once

#include "IMessage.h"
#include "../common/ThreadSafeQueue.h"
#include "Statistics.h" // For performance analysis
#include <memory>
#include <functional>
#include <string>
#include <vector>

namespace mini_ros {

// Forward declare
class Node;

// Base class for type erasure
class ISubscriber {
public:
    virtual ~ISubscriber() = default;
    virtual void spinOnce() = 0; // Polls the queue and fires callback
    virtual std::string getTopicName() const = 0;
    virtual void enqueueRaw(std::shared_ptr<IMessage> msg) = 0;
    virtual Statistics getStats() const = 0;
};

// Templated implementation
template<class MsgT>
class Subscriber : public ISubscriber {
public:
    Subscriber(const std::string& topic, std::function<void(std::shared_ptr<MsgT>)> callback)
        : topicName_(topic), callback_(callback) {}

    void spinOnce() override {
        std::shared_ptr<IMessage> rawMsg;
        if (queue_.try_pop(rawMsg)) {
            // Performance analysis
            Stopwatch sw;
            auto msg = std::dynamic_pointer_cast<MsgT>(rawMsg);
            if (msg) {
                callback_(msg);
            }
            stats_.add(sw.elapsed());
            
            // Calculate latency
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> latency = now - rawMsg->timestamp;
            latencyStats_.add(latency.count());
        }
    }

    void enqueueRaw(std::shared_ptr<IMessage> msg) override {
        queue_.push(msg);
    }
    
    std::string getTopicName() const override { return topicName_; }
    Statistics getStats() const override { return stats_; }
    Statistics getLatencyStats() const { return latencyStats_; }

private:
    std::string topicName_;
    std::function<void(std::shared_ptr<MsgT>)> callback_;
    ThreadSafeQueue<std::shared_ptr<IMessage>> queue_;
    Statistics stats_; // Callback duration stats
    Statistics latencyStats_; // End-to-end latency stats
};

} // namespace mini_ros