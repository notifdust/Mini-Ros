#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <cstring> // For memcpy
#include <chrono>

namespace mini_ros {

// Base interface for all messages
struct IMessage {
    // Timestamp for performance tracking
    std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;

    IMessage() : timestamp(std::chrono::high_resolution_clock::now()) {}
    virtual ~IMessage() = default;
};

// Example standard message
struct StringMessage : public IMessage {
    std::string data;

    // Simple "serialization" for demo
    // In a real system, this would be more robust (e.g., Protobuf, flatbuffers)
    std::vector<uint8_t> serialize() const {
        std::vector<uint8_t> buffer(data.size());
        std::memcpy(buffer.data(), data.c_str(), data.size());
        return buffer;
    }

    void deserialize(const std::vector<uint8_t>& buffer) {
        data.assign(buffer.begin(), buffer.end());
    }
};

// Example standard message
struct Int64Message : public IMessage {
    int64_t data;

    std::vector<uint8_t> serialize() const {
        std::vector<uint8_t> buffer(sizeof(data));
        std::memcpy(buffer.data(), &data, sizeof(data));
        return buffer;
    }

    void deserialize(const std::vector<uint8_t>& buffer) {
        if (buffer.size() == sizeof(data)) {
            std::memcpy(&data, buffer.data(), sizeof(data));
        }
    }
};

} // namespace mini_ros