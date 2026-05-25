#pragma once

#include "RtosAbstract.hpp"
#include "tx_api.h"
#include <array>

namespace rtos::threadx {

// ThreadX utilise des byte pools pour l'allocation statique
class MemoryManager {
public:
    static TX_BYTE_POOL* get_pool() {
        static TX_BYTE_POOL pool;
        static std::array<uint8_t, 32768> pool_memory; // 32KB pool statique
        static bool initialized = false;
        if (!initialized) {
            tx_byte_pool_create(&pool, (CHAR*)"SystemPool", pool_memory.data(), pool_memory.size());
            initialized = true;
        }
        return &pool;
    }
};

template<size_t StackSizeBytes>
class Task {
public:
    Task(const char* name, void (*func)(ULONG), ULONG param, UINT priority)
        : name_(name), func_(func), param_(param), priority_(priority) {
        static_assert(StackSizeBytes > 0, "Stack size must be > 0");
    }

    hal::Result<void> start() noexcept {
        void* stack_ptr = nullptr;
        if (tx_byte_allocate(MemoryManager::get_pool(), &stack_ptr, StackSizeBytes, TX_NO_WAIT) != TX_SUCCESS) {
            return std::unexpected(hal::HalError::Error);
        }

        UINT status = tx_thread_create(
            &thread_, (CHAR*)name_, func_, param_,
            stack_ptr, StackSizeBytes, priority_, priority_,
            TX_NO_TIME_SLICE, TX_AUTO_START
        );
        return status == TX_SUCCESS ? hal::Result<void>{} : std::unexpected(hal::HalError::Error);
    }

private:
    const char* name_;
    void (*func_)(ULONG);
    ULONG param_;
    UINT priority_;
    TX_THREAD thread_;
};

template<typename T, size_t Depth>
class Queue {
public:
    Queue() {
        void* queue_ptr = nullptr;
        tx_byte_allocate(MemoryManager::get_pool(), &queue_ptr, Depth * sizeof(T), TX_NO_WAIT);
        tx_queue_create(&handle_, (CHAR*)"Queue", sizeof(T)/4, queue_ptr, Depth * sizeof(T));
    }

    hal::Result<void> push(const T& msg, hal::Microseconds timeout) noexcept {
        UINT status = tx_queue_send(&handle_, (void*)&msg, timeout.count / 1000);
        return status == TX_SUCCESS ? hal::Result<void>{} : std::unexpected(hal::HalError::Timeout);
    }

    hal::Result<T> pop(hal::Microseconds timeout) noexcept {
        T msg;
        UINT status = tx_queue_receive(&handle_, (void*)&msg, timeout.count / 1000);
        return status == TX_SUCCESS ? hal::Result<T>{msg} : std::unexpected(hal::HalError::Timeout);
    }

private:
    TX_QUEUE handle_;
};

} // namespace rtos::threadx
