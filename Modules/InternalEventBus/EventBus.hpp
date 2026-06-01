#pragma once

#include "RtosAbstract.hpp"
#include <array>
#include <atomic>
#include <cassert>

namespace bus {

// Interface pour éviter l'usage direct du concept comme type
template<typename T>
class ISubscriber {
public:
    virtual hal::Result<void> push(const T& msg, hal::Microseconds timeout) noexcept = 0;
};

// Adaptateur statique pour lier IQueue au ISubscriber
template<typename T, typename QueueType>
class QueueAdapter : public ISubscriber<T> {
public:
    explicit QueueAdapter(QueueType& q) : queue_(q) {}
    hal::Result<void> push(const T& msg, hal::Microseconds timeout) noexcept override {
        return queue_.push(msg, timeout);
    }
private:
    QueueType& queue_;
};

template<typename T, size_t Subscribers = 8>
class EventBus {
public:
    hal::Result<void> publish(const T& msg) noexcept {
        bool busy_detected = false;
        for (size_t i = 0; i < subscriber_count_; ++i) {
            auto result = subscribers_[i]->push(msg, hal::Microseconds(1000));
            if (!result) {
                busy_detected = true;
            }
        }
        if (busy_detected) return std::unexpected(hal::HalError::Busy);
        return {};
    }

    // L'abonné doit fournir un objet implémentant ISubscriber (ex: QueueAdapter)
    // dont la durée de vie est garantie supérieure à celle du bus.
    void subscribe(ISubscriber<T>& subscriber) {
        assert(!locked_ && "Cannot subscribe after initialization");
        if (subscriber_count_ < Subscribers) {
            subscribers_[subscriber_count_++] = &subscriber;
        }
    }

    void finalize_init() {
        locked_ = true;
    }

private:
    bool locked_ = false;
    std::array<ISubscriber<T>*, Subscribers> subscribers_{};
    size_t subscriber_count_ = 0;
};

} // namespace bus
