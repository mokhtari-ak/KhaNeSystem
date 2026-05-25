#pragma once

#include "RtosAbstract.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

namespace rtos::freertos {

template<size_t StackSizeBytes>
class Task {
public:
    Task(const char* name, void (*func)(void*), void* param, UBaseType_t priority)
        : name_(name), func_(func), param_(param), priority_(priority) {
        static_assert(StackSizeBytes > 0, "Stack size must be > 0");
    }

    hal::Result<void> start() noexcept {
        stack_buffer_.fill(0);
        handle_ = xTaskCreateStatic(
            func_, name_, StackSizeBytes / sizeof(StackType_t), param_,
            priority_, stack_buffer_.data(), &tcb_
        );
        return handle_ ? hal::Result<void>{} : std::unexpected(hal::HalError::Error);
    }

private:
    const char* name_;
    void (*func_)(void*);
    void* param_;
    UBaseType_t priority_;
    StaticTask_t tcb_;
    std::array<StackType_t, StackSizeBytes / sizeof(StackType_t)> stack_buffer_;
    TaskHandle_t handle_;
};

} // namespace rtos::freertos
