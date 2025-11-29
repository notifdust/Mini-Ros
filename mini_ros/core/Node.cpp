#include "Node.h"
#include "MiniRosCore.h"

namespace mini_ros {

Node::Node(const std::string& name) : name_(name) {
    core_ = &MiniRosCore::getInstance();
    core_->registerNode(this);
}

Node::~Node() {
    shutdown();
    core_->unregisterNode(this);
}

void Node::addSubscriber(std::shared_ptr<ISubscriber> sub) {
    subscribers_.push_back(sub);
    core_->registerSubscriber(sub);
}

void Node::addServiceServer(std::shared_ptr<IServiceServer> server) {
    serviceServers_.push_back(server);
    core_->registerServiceServer(server);
}

void Node::spin() {
    while (ok()) {
        spinOnce();
        // In a real-time system, you'd sleep for a precise duration here
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void Node::spinOnce() {
    // Process all subscriber callbacks
    for (auto& sub : subscribers_) {
        sub->spinOnce();
    }
    
    // Process all service server callbacks
    for (auto& server : serviceServers_) {
        server->spinOnce();
    }
    
    // Process all timers
    for (auto& timer : timers_) {
        timer->spinOnce();
    }
}

void Node::shutdown() {
    running_ = false;
}

bool Node::ok() const {
    return running_ && core_->ok();
}

} // namespace mini_ros