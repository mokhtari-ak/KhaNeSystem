#pragma once

#include "RtosAbstract.hpp"
#include <array>
#include <atomic>

namespace bus {

// Messages doivent avoir une taille fixe et être triviaux
struct alignas(4) MessageBase {
    uint32_t version;
};

// EventBus gérant la distribution typée des messages
template<typename T, size_t Subscribers = 4>
class EventBus {
public:
    // Publication d'un message vers tous les abonnés
    hal::Result<void> publish(const T& msg) noexcept {
        for (size_t i = 0; i < subscriber_count_; ++i) {
            auto result = subscribers_[i]->push(msg, hal::Microseconds(1000));
            if (!result) return std::unexpected(hal::HalError::Error);
        }
        return {};
    }

    // Enregistrement d'un abonné (appelé uniquement au boot)
    void subscribe(rtos::IQueue<T, 8>& queue) {
        if (subscriber_count_ < Subscribers) {
            subscribers_[subscriber_count_++] = &queue;
        }
    }

private:
    std::array<rtos::IQueue<T, 8>*, Subscribers> subscribers_{};
    size_t subscriber_count_ = 0;
};

} // namespace bus
