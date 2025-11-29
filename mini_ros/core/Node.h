#pragma once

#include "Publisher.h"
#include "Subscriber.h"
#include "ServiceClient.h"
#include "ServiceServer.h"
#include "Timer.h"
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <atomic>

namespace mini_ros {

// Forward declare
class MiniRosCore;

class Node {
public:
    Node(const std::string& name);
    ~Node();

    // Factory methods for creating ROS primitives
    template<class MsgT>
    std::shared_ptr<Publisher> createPublisher(const std::string& topic) {
        auto pub = std::make_shared<Publisher>(topic, core_);
        publishers_.push_back(pub);
        return pub;
    }

    template<class MsgT>
    std::shared_ptr<Subscriber<MsgT>> createSubscriber(
        const std::string& topic, 
        std::function<void(std::shared_ptr<MsgT>)> callback
    ) {
        auto sub = std::make_shared<Subscriber<MsgT>>(topic, callback);
        addSubscriber(sub);
        return sub;
    }

    template<class SrvT>
    std::shared_ptr<ServiceServer<SrvT>> createServiceServer(
        const std::string& service,
        typename ServiceServer<SrvT>::CallbackT callback
    ) {
        auto server = std::make_shared<ServiceServer<SrvT>>(service, callback);
        addServiceServer(server);
        return server;
    }

    template<class SrvT>
    std::shared_ptr<ServiceClient> createServiceClient(const std::string& service) {
        auto client = std::make_shared<ServiceClient>(service, core_);
        serviceClients_.push_back(client);
        return client;
    }

    std::shared_ptr<Timer> createTimer(std::chrono::duration<double> period, Timer::CallbackT callback) {
        auto timer = std::make_shared<Timer>(period, callback);
        timers_.push_back(timer);
        return timer;
    }

    // Main scheduler loop
    void spin();
    
    // Single spin iteration
    void spinOnce();

    // Shutdown
    void shutdown();
    bool ok() const;

    std::string getName() const { return name_; }

private:
    void addSubscriber(std::shared_ptr<ISubscriber> sub);
    void addServiceServer(std::shared_ptr<IServiceServer> server);

    std::string name_;
    MiniRosCore* core_; // Raw pointer to singleton
    
    // Lists of schedulable items
    std::vector<std::shared_ptr<Publisher>> publishers_;
    std::vector<std::shared_ptr<ISubscriber>> subscribers_;
    std::vector<std::shared_ptr<ServiceClient>> serviceClients_;
    std::vector<std::shared_ptr<IServiceServer>> serviceServers_;
    std::vector<std::shared_ptr<Timer>> timers_;
    
    std::atomic<bool> running_{true};
};

} // namespace mini_ros