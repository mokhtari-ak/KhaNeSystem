#pragma once
#include "IHalWrappers.hpp"
#include <vector>

namespace hal {

class MockSpiWrapper {
public:
    Result<void> transmit_receive(std::span<uint8_t> tx, std::span<uint8_t> rx, Microseconds /*timeout*/) noexcept {
        // Mock implementation for unit tests
        for (size_t i = 0; i < rx.size(); ++i) {
            rx[i] = 0xAA; // Dummy data
        }
        return {};
    }
};

} // namespace hal
