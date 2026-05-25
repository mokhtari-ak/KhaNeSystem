#pragma once

#include <cstddef>
#include <cstdint>
#include "HalTypes.hpp"

namespace rtos {

// Spécification de taille de pile (Strong Type)
struct StackSize {
    size_t bytes;
    explicit constexpr StackSize(size_t s) : bytes(s) {}
};

// Interface concept pour une tâche RTOS
template<typename T>
concept ITask = requires(T t) {
    { t.start() } -> std::same_as<hal::Result<void>>;
    { t.suspend() } -> std::same_as<hal::Result<void>>;
    { t.resume() } -> std::same_as<hal::Result<void>>;
};

// Interface concept pour une file de messages
template<typename T, typename MessageType>
concept IQueue = requires(T t, MessageType msg, hal::Microseconds timeout) {
    { t.push(msg, timeout) } -> std::same_as<hal::Result<void>>;
    { t.pop(timeout) } -> std::same_as<hal::Result<MessageType>>;
};

} // namespace rtos
