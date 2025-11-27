#include "mini_ros/core/Node.h"
#include "mini_ros/core/MiniRosCore.h"
#include "mini_ros/core/StdMessages.h"
#include <iostream>
#include <thread>

using namespace mini_ros;

// Listener node's callback
void chatterCallback(std::shared_ptr<StringMessage> msg) {
    std::cout << "Listener heard: [" << msg->data << "]" << std::endl;
}

int main() {
    // Create nodes
    Node talker_node("talker");
    Node listener_node("listener");

    // Talker creates a publisher
    auto chatter_pub = talker_node.createPublisher<StringMessage>("chatter");
    
    // Listener creates a subscriber
    auto chatter_sub = listener_node.createSubscriber<StringMessage>("chatter", &chatterCallback);

    // Run the listener node in a separate thread
    std::thread listener_thread([&]() {
        listener_node.spin();
    });

    // Run the talker logic in the main thread
    int count = 0;
    while (talker_node.ok() && count < 10) {
        auto msg = std::make_shared<StringMessage>();
        msg->data = "Hello Mini-ROS! " + std::to_string(count++);
        
        std::cout << "Talker says: [" << msg->data << "]" << std::endl;
        chatter_pub->publish(msg);
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Shutdown
    MiniRosCore::getInstance().shutdown();
    listener_thread.join();
    
    std::cout << "Talker/Listener demo finished." << std::endl;
    return 0;
}