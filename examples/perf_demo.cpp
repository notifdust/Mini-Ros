#include "mini_ros/core/Node.h"
#include "mini_ros/core/MiniRosCore.h"
#include "mini_ros/core/StdMessages.h"
#include <iostream>
#include <thread>
#include <iomanip> // For std::setprecision

using namespace mini_ros;

std::shared_ptr<Subscriber<Int64Message>> sub;
std::shared_ptr<Timer> timer;

void perfCallback(std::shared_ptr<Int64Message> msg) {
    // Do a tiny bit of "work"
    double val = msg->data;
    val = std::sin(val) * std::cos(val);
}

void timerCallback() {
    // This timer just prints stats
    if (!sub || !timer) return;

    auto subStats = sub->getStats();
    auto latencyStats = sub->getLatencyStats();
    auto timerStats = timer->getStats();
    
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "--- Performance Stats ---" << std::endl;
    std::cout << "Subscriber Callback (us): "
              << "mean=" << subStats.mean() * 1e6
              << " max=" << subStats.max() * 1e6
              << " stddev=" << subStats.stddev() * 1e6 << std::endl;
              
    std::cout << "Message Latency (ms):     "
              << "mean=" << latencyStats.mean()
              << " max=" << latencyStats.max()
              << " stddev=" << latencyStats.stddev() << std::endl;
              
    std::cout << "Timer Callback (us):      "
              << "mean=" << timerStats.mean() * 1e6
              << " max=" << timerStats.max() * 1e6 << std::endl;
    std::cout << "Total Msgs: " << subStats.count() << std::endl;
    std::cout << "-------------------------" << std::endl;
}

int main() {
    Node publisher_node("publisher");
    Node listener_node("listener");

    auto pub = publisher_node.createPublisher<Int64Message>("perf_topic");
    sub = listener_node.createSubscriber<Int64Message>("perf_topic", &perfCallback);
    
    // Create a timer in the listener node to print stats
    timer = listener_node.createTimer(std::chrono::seconds(2), &timerCallback);

    // Run listener in its own thread
    std::thread listener_thread([&]() {
        listener_node.spin();
    });

    // Run publisher in main thread (very fast!)
    int64_t count = 0;
    while (publisher_node.ok() && count < 10000) {
        auto msg = std::make_shared<Int64Message>();
        msg->data = count++;
        pub->publish(msg);
        // Sleep for 100 microseconds
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(1));

    MiniRosCore::getInstance().shutdown();
    listener_thread.join();
    
    std::cout << "Performance demo finished." << std::endl;
    return 0;
}