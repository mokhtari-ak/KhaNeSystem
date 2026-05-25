#pragma once

#include "stm32f4xx_hal.h"
#include "HalTypes.hpp"
#include <array>

namespace hal {

struct DacInstance {
    DAC_HandleTypeDef handle;
    bool initialized = false;
};

class DacDriver {
public:
    static constexpr size_t MaxInstances = 2; // DAC1, DAC2

    template<typename Config>
    static Result<uint8_t> init() noexcept {
        static std::array<DacInstance, MaxInstances> instances{};
        return 0;
    }

    static Result<void> set_value(uint8_t index, uint32_t alignment, uint32_t value) noexcept {
        return {};
    }
};

} // namespace hal
