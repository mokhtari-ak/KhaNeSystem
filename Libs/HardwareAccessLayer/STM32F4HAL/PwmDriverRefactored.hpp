#pragma once

#include "stm32f4xx_hal.h"
#include "HalTypes.hpp"
#include <array>

namespace hal {

struct PwmInstance {
    TIM_HandleTypeDef handle;
    bool initialized = false;
};

class PwmDriver {
public:
    static constexpr size_t MaxInstances = 14; // TIM1..14

    template<typename Config>
    static Result<uint8_t> init() noexcept {
        static std::array<PwmInstance, MaxInstances> instances{};
        return 0;
    }

    static Result<void> set_duty_cycle(uint8_t index, uint32_t channel, uint32_t value) noexcept {
        return {};
    }
};

} // namespace hal
