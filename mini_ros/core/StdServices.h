#pragma once

#include "IService.h"

namespace mini_ros {

// Example Service: AddTwoInts
struct AddTwoInts : public IService {
    
    struct Request {
        int64_t a;
        int64_t b;
    };

    struct Response {
        int64_t sum;
    };

    // Type aliases for convenience
    using RequestPtr = std::shared_ptr<Request>;
    using ResponsePtr = std::shared_ptr<Response>;
};

} // namespace mini_ros