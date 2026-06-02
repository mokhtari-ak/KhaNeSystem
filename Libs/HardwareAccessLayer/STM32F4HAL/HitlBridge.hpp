#pragma once

#include "ISimulationBridge.hpp"
#include "IUartDriver.hpp"
#include "HalTypes.hpp"

namespace hal {

/**
 * @brief Implémentation du pont HITL utilisant UART avec DMA.
 */
class HitlBridge : public ISimulationBridge {
public:
    explicit HitlBridge(IUartDriver& uart) : uart_(uart) {}

    Result<void> send(std::span<const uint8_t> data) noexcept override {
        return uart_.transmit(data);
    }

    Result<size_t> receive(std::span<uint8_t> buffer) noexcept override {
        // Lecture non-bloquante avec timeout court ou polling selon config UART
        return uart_.receive(buffer);
    }

    bool is_connected() const noexcept override {
        return uart_.is_ready();
    }

private:
    IUartDriver& uart_;
};

} // namespace hal
