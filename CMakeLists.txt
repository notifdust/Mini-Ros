cmake_minimum_required(VERSION 3.10)
project(MiniROS CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find pthreads, which is what std::thread relies on
find_package(Threads REQUIRED)

# --- Define the Mini-ROS Library ---
# This library will contain all the core logic.
add_library(mini_ros
    mini_ros/core/MiniRosCore.cpp
    mini_ros/core/Node.cpp
    mini_ros/core/Publisher.cpp
    mini_ros/core/Subscriber.cpp
    mini_ros/core/ServiceServer.cpp
    mini_ros/core/ServiceClient.cpp
    mini_ros/core/Timer.cpp
)

# Public include directories for the library
target_include_directories(mini_ros PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link the library against pthreads
target_link_libraries(mini_ros PUBLIC Threads::Threads)

# --- Example: Talker/Listener (Pub/Sub) ---
add_executable(talker_listener examples/talker_listener.cpp)
target_link_libraries(talker_listener mini_ros)

# --- Example: Service Client/Server ---
add_executable(service_client_server examples/service_client_server.cpp)
target_link_libraries(service_client_server mini_ros)

# --- Example: Performance Analysis ---
add_executable(perf_demo examples/perf_demo.cpp)
target_link_libraries(perf_demo mini_ros)