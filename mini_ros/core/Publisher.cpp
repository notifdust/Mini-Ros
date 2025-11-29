#include "Publisher.h"
#include "MiniRosCore.h" // Include the full definition here

namespace mini_ros {

Publisher::Publisher(const std::string& topic, MiniRosCore* core)
    : topicName_(topic), core_(core) {
    // Register this publisher with the core
    core_->registerPublisher(this, topicName_);
}

void Publisher::doPublish(std::shared_ptr<IMessage> msg) {
    core_->publish(topicName_, msg);
}

} // namespace mini_ros