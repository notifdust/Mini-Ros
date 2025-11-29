#include "MiniRosCore.h"
#include "Node.h" // For Node::shutdown

namespace mini_ros {

void MiniRosCore::registerNode(Node* node) {
    std::lock_guard<std::mutex> lock(nodeMutex_);
    nodes_.push_back(node);
}

void MiniRosCore::unregisterNode(Node* node) {
    std::lock_guard<std::mutex> lock(nodeMutex_);
    nodes_.erase(std::remove(nodes_.begin(), nodes_.end(), node), nodes_.end());
}

void MiniRosCore::registerPublisher(Publisher* pub, const std::string& topic) {
    // We don't strictly need to register publishers in this model,
    // but we could add logging or tracking here.
}

void MiniRosCore::registerSubscriber(std::shared_ptr<ISubscriber> sub) {
    std::lock_guard<std::mutex> lock(topicMutex_);
    topicSubscribers_[sub->getTopicName()].push_back(sub);
}

void MiniRosCore::registerServiceServer(std::shared_ptr<IServiceServer> server) {
    std::lock_guard<std::mutex> lock(serviceMutex_);
    // Assume one server per service
    serviceServers_[server->getServiceName()] = server;
}

std::shared_ptr<IServiceServer> MiniRosCore::findService(const std::string& serviceName) {
    std::lock_guard<std::mutex> lock(serviceMutex_);
    auto it = serviceServers_.find(serviceName);
    if (it != serviceServers_.end()) {
        if (auto server = it->second.lock()) {
            return server;
        } else {
            // Clean up expired server
            serviceServers_.erase(it);
        }
    }
    return nullptr;
}

void MiniRosCore::publish(const std::string& topic, std::shared_ptr<IMessage> msg) {
    if (!ok()) return;
    
    std::lock_guard<std::mutex> lock(topicMutex_);
    auto it = topicSubscribers_.find(topic);
    if (it != topicSubscribers_.end()) {
        auto& subs = it->second;
        // Clean up expired subscribers while iterating
        subs.erase(std::remove_if(subs.begin(), subs.end(),
            [&](std::weak_ptr<ISubscriber>& w_sub) {
                if (auto sub = w_sub.lock()) {
                    sub->enqueueRaw(msg); // This is the "transport"
                    return false; // Keep
                }
                return true; // Remove
            }),
            subs.end());
    }
}

void MiniRosCore::shutdown() {
    running_ = false;
    std::lock_guard<std::mutex> lock(nodeMutex_);
    for (auto* node : nodes_) {
        node->shutdown();
    }
}

bool MiniRosCore::ok() const {
    return running_;
}

} // namespace mini_ros