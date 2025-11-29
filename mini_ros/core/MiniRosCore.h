#pragma once

#include "Subscriber.h"
#include "ServiceServer.h"
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <memory>
#include <atomic>

namespace mini_ros {

// Forward declares
class Node;
class Publisher;

// MiniRosCore is a singleton that manages discovery
class MiniRosCore {
public:
    static MiniRosCore& getInstance() {
        static MiniRosCore instance;
        return instance;
    }

    // Public API for Nodes
    void registerNode(Node* node);
    void unregisterNode(Node* node);

    void registerPublisher(Publisher* pub, const std::string& topic);
    void registerSubscriber(std::shared_ptr<ISubscriber> sub);
    
    void registerServiceServer(std::shared_ptr<IServiceServer> server);
    std::shared_ptr<IServiceServer> findService(const std::string& serviceName);
    
    // Called by Publishers
    void publish(const std::string& topic, std::shared_ptr<IMessage> msg);

    // Global shutdown
    void shutdown();
    bool ok() const;

private:
    MiniRosCore() = default;
    ~MiniRosCore() = default;
    MiniRosCore(const MiniRosCore&) = delete;
    MiniRosCore& operator=(const MiniRosCore&) = delete;

    std::atomic<bool> running_{true};

    // Registries
    std::mutex nodeMutex_;
    std::vector<Node*> nodes_;

    std::mutex topicMutex_;
    // We don't need to store publishers, just subscribers
    std::map<std::string, std::vector<std::weak_ptr<ISubscriber>>> topicSubscribers_;

    std::mutex serviceMutex_;
    std::map<std::string, std::weak_ptr<IServiceServer>> serviceServers_;
};

} // namespace mini_ros