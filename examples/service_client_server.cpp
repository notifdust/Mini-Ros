#include "mini_ros/core/Node.h"
#include "mini_ros/core/MiniRosCore.h"
#include "mini_ros/core/StdServices.h"
#include <iostream>
#include <thread>

using namespace mini_ros;

// Service server callback
bool add(AddTwoInts::RequestPtr req, AddTwoInts::ResponsePtr res) {
    std::cout << "Server received request: " << req->a << " + " << req->b << std::endl;
    res->sum = req->a + req->b;
    std::cout << "Server sending response: " << res->sum << std::endl;
    return true;
}

int main() {
    Node server_node("add_server");
    Node client_node("add_client");

    // Server creates a service
    auto server = server_node.createServiceServer<AddTwoInts>("add_two_ints", &add);

    // Client creates a client
    auto client = client_node.createServiceClient<AddTwoInts>("add_two_ints");

    // Run the server node in a separate thread
    std::thread server_thread([&]() {
        server_node.spin();
    });

    // Give the server a moment to register
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Run the client logic in the main thread
    auto req = std::make_shared<AddTwoInts::Request>();
    req->a = 10;
    req->b = 20;

    auto res = std::make_shared<AddTwoInts::Response>();

    if (client->call(req, res)) {
        std::cout << "Client received sum: " << res->sum << std::endl;
    } else {
        std::cout << "Client failed to call service." << std::endl;
    }

    // Shutdown
    MiniRosCore::getInstance().shutdown();
    server_thread.join();
    
    std::cout << "Service demo finished." << std::endl;
    return 0;
}