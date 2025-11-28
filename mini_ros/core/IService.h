#pragma once

#include <memory>

namespace mini_ros {

// Base class for a service
struct IService {
    // We need shared_ptr for the request and response
    using RequestPtr = std::shared_ptr<void>;
    using ResponsePtr = std::shared_ptr<void>;
    
    virtual ~IService() = default;
};

} // namespace mini_ros