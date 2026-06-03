#pragma once

#include "RtosAbstract.hpp"
#include "tx_api.h"
#include <array>

namespace rtos::threadx {

template<size_t StackSizeBytes>
class Task {
public:
    Task(const char* name, void (*func)(ULONG), ULONG param, UINT priority)
        : name_(name), func_(func), param_(param), priority_(priority) {
        static_assert(StackSizeBytes > 0, "Stack size must be > 0");
    }

    hal::Result<void> start() noexcept {
        UINT status = tx_thread_create(
            &thread_, (CHAR*)name_, func_, param_,
            stack_buffer_.data(), StackSizeBytes, priority_, priority_,
            TX_NO_TIME_SLICE, TX_AUTO_START
        );
        return status == TX_SUCCESS ? hal::Result<void>{} : hal::unexpected(hal::HalError::Error);
    }

private:
    const char* name_;
    void (*func_)(ULONG);
    ULONG param_;
    UINT priority_;
    TX_THREAD thread_;
    std::array<uint8_t, StackSizeBytes> stack_buffer_;
};

template<typename T, size_t Depth>
class Queue {
public:
    Queue() {
        tx_queue_create(&handle_, (CHAR*)"Queue", sizeof(T)/sizeof(ULONG), buffer_.data(), sizeof(buffer_));
    }

    hal::Result<void> push(const T& msg, hal::Microseconds timeout) noexcept {
        UINT status = tx_queue_send(&handle_, (void*)&msg, timeout.count / 1000);
        return status == TX_SUCCESS ? hal::Result<void>{} : hal::unexpected(hal::HalError::Timeout);
    }

    hal::Result<T> pop(hal::Microseconds timeout) noexcept {
        T msg;
        UINT status = tx_queue_receive(&handle_, (void*)&msg, timeout.count / 1000);
        return status == TX_SUCCESS ? hal::Result<T>{msg} : hal::unexpected(hal::HalError::Timeout);
    }

private:
    TX_QUEUE handle_;
    std::array<uint8_t, Depth * sizeof(T)> buffer_;
};

} // namespace rtos::threadx
