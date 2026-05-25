#pragma once

#include "stm32f4xx_hal.h"
#include "HalTypes.hpp"
#include <array>

namespace hal {

struct CanInstance {
    CAN_HandleTypeDef handle;
    bool initialized = false;
};

class CanDriver {
public:
    static constexpr size_t MaxInstances = 2; // CAN1, CAN2

    template<typename Config>
    static Result<uint8_t> init() noexcept {
        static std::array<CanInstance, MaxInstances> instances{};
        return 0;
    }

    static Result<void> transmit(uint8_t index, const CAN_TxHeaderTypeDef& header, const uint8_t* data, uint32_t* mailbox) noexcept {
        return {};
    }

    static Result<void> receive(uint8_t index, uint32_t fifo, CAN_RxHeaderTypeDef& header, uint8_t* data) noexcept {
        return {};
    }
};

} // namespace hal
