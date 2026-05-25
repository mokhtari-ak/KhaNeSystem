#pragma once

#include "IHalWrappers.hpp"
#include "SpiDriverRefactored.hpp"
#include <span>

namespace hal {

// Adaptateur pour faire correspondre l'interface du driver aux concepts attendus
class SpiStaticAdapter {
public:
    template<typename Config>
    explicit SpiStaticAdapter() : index_(0) {
        auto result = SpiDriver::init<Config>();
        if (result) {
            index_ = *result;
        }
    }

    Result<void> transmit_receive(std::span<uint8_t> tx, std::span<uint8_t> rx, Microseconds timeout) noexcept {
        return SpiDriver::transmit_receive(index_, tx, rx, timeout);
    }

private:
    uint8_t index_;
};

// Vérification statique de la conformité au concept
static_assert(ISpiWrapper<SpiStaticAdapter>, "SpiStaticAdapter must satisfy ISpiWrapper concept");

} // namespace hal
