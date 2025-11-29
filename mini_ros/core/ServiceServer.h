#pragma once

#include "IService.h"
#include "../common/ThreadSafeQueue.h"
#include "Statistics.h" // For performance analysis
#include <string>
#include <functional>
#include <memory>
#include <future> // For pending calls

namespace mini_ros {

// A struct to hold a pending service call
struct PendingCall {
    IService::RequestPtr request;
    std::promise<IService::ResponsePtr> promise;
};

// Base class for type erasure
class IServiceServer {
public:
    virtual ~IServiceServer() = default;
    virtual void spinOnce() = 0;
    virtual std::string getServiceName() const = 0;
    // Called by a ServiceClient
    virtual std::future<IService::ResponsePtr> enqueueCall(IService::RequestPtr req) = 0;
    virtual Statistics getStats() const = 0;
};

template<class SrvT>
class ServiceServer : public IServiceServer {
public:
    using RequestPtr = typename SrvT::RequestPtr;
    using ResponsePtr = typename SrvT::ResponsePtr;
    using CallbackT = std::function<bool(RequestPtr, ResponsePtr)>;

    ServiceServer(const std::string& serviceName, CallbackT callback)
        : serviceName_(serviceName), callback_(callback) {}

    void spinOnce() override {
        std::shared_ptr<PendingCall> call;
        if (queue_.try_pop(call)) {
            Stopwatch sw;
            auto req = std::static_pointer_cast<typename SrvT::Request>(call->request);
            auto res = std::make_shared<typename SrvT::Response>();
            
            bool success = callback_(req, res);
            stats_.add(sw.elapsed());

            if (success) {
                call->promise.set_value(res);
            } else {
                // Set an exception or a null response to indicate failure
                call->promise.set_value(nullptr); 
            }
        }
    }

    std::future<IService::ResponsePtr> enqueueCall(IService::RequestPtr req) override {
        auto call = std::make_shared<PendingCall>();
        call->request = req;
        queue_.push(call);
        return call->promise.get_future();
    }

    std::string getServiceName() const override { return serviceName_; }
    Statistics getStats() const override { return stats_; }

private:
    std::string serviceName_;
    CallbackT callback_;
    ThreadSafeQueue<std::shared_ptr<PendingCall>> queue_;
    Statistics stats_; // Callback duration stats
};

} // namespace mini_ros