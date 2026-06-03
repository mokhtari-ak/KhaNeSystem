#pragma once

#include "RtosAbstract.hpp"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include <array>

namespace rtos::freertos {

template<typename T, size_t Depth>
class Queue {
public:
    Queue() {
        handle_ = xQueueCreateStatic(Depth, sizeof(T), buffer_.data(), &queue_struct_);
    }

    hal::Result<void> push(const T& msg, hal::Microseconds timeout) noexcept {
        if (xQueueSend(handle_, &msg, pdMS_TO_TICKS(timeout.count / 1000)) == pdPASS) {
            return {};
        }
        return hal::unexpected(hal::HalError::Timeout);
    }

    hal::Result<T> pop(hal::Microseconds timeout) noexcept {
        T msg;
        if (xQueueReceive(handle_, &msg, pdMS_TO_TICKS(timeout.count / 1000)) == pdPASS) {
            return msg;
        }
        return hal::unexpected(hal::HalError::Timeout);
    }

private:
    QueueHandle_t handle_;
    StaticQueue_t queue_struct_;
    std::array<uint8_t, Depth * sizeof(T)> buffer_;
};

class BinarySemaphore {
public:
    BinarySemaphore() {
        handle_ = xSemaphoreCreateBinaryStatic(&buffer_);
    }

    hal::Result<void> give() noexcept {
        return xSemaphoreGive(handle_) == pdTRUE ? hal::Result<void>{} : hal::unexpected(hal::HalError::Error);
    }

    hal::Result<void> take(hal::Microseconds timeout) noexcept {
        return xSemaphoreTake(handle_, pdMS_TO_TICKS(timeout.count / 1000)) == pdTRUE ? hal::Result<void>{} : hal::unexpected(hal::HalError::Timeout);
    }

private:
    SemaphoreHandle_t handle_;
    StaticSemaphore_t buffer_;
};

} // namespace rtos::freertos
