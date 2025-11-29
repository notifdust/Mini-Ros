#pragma once

#include "IMessage.h"
#include <string>
#include <memory>

namespace mini_ros {

// Forward declare
class MiniRosCore;

class Publisher {
public:
    // Publisher is created by a Node, which passes a reference to the core
    Publisher(const std::string& topic, MiniRosCore* core);

    template<class MsgT>
    void publish(std::shared_ptr<MsgT> msg) {
        // Set timestamp right before publishing
        msg->timestamp = std::chrono::high_resolution_clock::now();
        doPublish(msg);
    }

private:
    void doPublish(std::shared_ptr<IMessage> msg);
    std::string topicName_;
    MiniRosCore* core_; // Raw pointer to the singleton core
};

} // namespace mini_ros