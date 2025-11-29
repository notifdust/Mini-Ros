#pragma once

#include "IService.h"
#include <string>
#include <memory>
#include <future>

namespace mini_ros {

// Forward declare
class MiniRosCore;
class IServiceServer;

class ServiceClient {
public:
    ServiceClient(const std::string& serviceName, MiniRosCore* core);

    template<class SrvT>
    bool call(typename SrvT::RequestPtr req, typename SrvT::ResponsePtr& res) {
        // Find the service server via the core
        auto server = findServer();
        if (!server) {
            return false; // Service not found
        }

        // Enqueue the call and get a future
        auto future = server->enqueueCall(req);

        // Block until the future is ready (synchronous call)
        // A real-time system might use a timeout
        future.wait();
        
        auto baseRes = future.get();
        if (baseRes) {
            res = std::static_pointer_cast<typename SrvT::Response>(baseRes);
            return true;
        }
        return false;
    }

private:
    std::shared_ptr<IServiceServer> findServer();
    std::string serviceName_;
    MiniRosCore* core_;
};

} // namespace mini_ros