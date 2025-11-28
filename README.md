# Mini-ROS: A Lightweight C++17 Robotics Framework


Mini-ROS is a compact, single-process, multi-threaded robotics framework that brings the essential concepts of ROS to small embedded systems. It is written entirely in C++17, avoids external dependencies, and is optimized for low latency and deterministic communication.


Its purpose is to deliver a clear software structure for robotics applications without requiring a full ROS installation, which is often impractical on lightweight controllers.


---


## Why Mini-ROS?
Modern robots require multiple components to run concurrently—sensors, control algorithms, estimators, actuators, safety checks. In a monolithic program, this becomes unmanageable.


Mini-ROS solves this by:
- Separating logic into **Nodes**
- Providing **standard communication patterns** (topics, services)
- Avoiding the network overhead of ROS 1/2
- Remaining **small, deterministic, and fast**


This makes it suitable for:
- Real-time control loops
- UAVs, mobile robots, small manipulators
- Robotics projects running on Raspberry Pi, Jetson Nano, or similar hardware


---


## Core Features
### 🔹 Topic-Based Pub/Sub
Nodes communicate via named topics:
- Publishers write messages to a topic
- Subscribers receive them asynchronously


Communication uses lock-free or low-lock thread-safe queues for minimal latency.


### 🔹 Services (Synchronous Request/Response)
Services allow structured, blocking communication between nodes. Example: a path planner responding with a computed trajectory.


### 🔹 Timers and Node Scheduler
Nodes support `spin()` loops or `spinOnce()`, handling:
- Topic callbacks
- Service callbacks
- Timer events


### 🔹 Real-Time Performance Tools
Included utilities:
- `Stopwatch` → measure callback durations
- `Statistics` → running average, variance, latency
- `ThreadSafeQueue` → high-speed message passing


These make debugging and performance validation easier.


---


## Architecture Overview
Mini-ROS uses a central `MiniRosCore` singleton for discovery. Once linked, nodes exchange data directly without going through the core.


*Advantages*:
- No serialization unless you add it
- No sockets, no DDS, no middle-layer
- Deterministic memory access


The framework is intentionally minimal. You keep full control of memory, threading, and timing.


---


## Project Structure
Mini-ROS/ ├── CMakeLists.txt ├── LICENSE ├── README.md ├── examples/ │ ├── perf_demo.cpp │ ├── service_client_server.cpp │ └── talker_listener.cpp └── mini_ros/ ├── common/ │ ├── Statistics.h │ ├── Stopwatch.h │ └── ThreadSafeQueue.h └── core/ ├── IMessage.h ├── IService.h ├── MiniRosCore.cpp ├── MiniRosCore.h ├── Node.cpp ├── Node.h ├── Publisher.cpp ├── Publisher.h ├── ServiceClient.cpp ├── ServiceClient.h ├── ServiceServer.h ├── Subscriber.h └── Timer.h



---


## Build Instructions
### Requirements
- **GCC ≥ 8** or **Clang ≥ 6**
- **CMake ≥ 3.10**
- Linux recommended (but works on Windows with MinGW or WSL)


### Steps
```bash
git clone https://github.com/your-username/mini-ros.git
cd mini-ros
mkdir build && cd build
cmake ..
make -j4
```

## Running the Examples
1. Publish/Subscribe Demo

```bash
./examples/talker_listener
```
Shows one node publishing data and another receiving it.

2. Service Client/Server Demo

```bash
./examples/service_client_server
```
Tests the synchronous request/response model.

3. Performance Benchmark Demo
```bash
./examples/perf_demo
```
Includes latency and throughput statistics.