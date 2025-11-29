#include "ServiceClient.h"
#include "MiniRosCore.h"

namespace mini_ros {

ServiceClient::ServiceClient(const std::string& serviceName, MiniRosCore* core)
    : serviceName_(serviceName), core_(core) {
}

std::shared_ptr<IServiceServer> ServiceClient::findServer() {
    return core_->findService(serviceName_);
}

} // namespace mini_ros