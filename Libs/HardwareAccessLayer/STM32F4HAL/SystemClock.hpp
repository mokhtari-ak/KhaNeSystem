#pragma once

#include "HalTypes.hpp"
#include <concepts>
#include <type_traits>
#include "stm32f4xx_hal.h"

namespace hal {

/**
 * @Brief Implémentation de SystemClock utilisant TIM2 (32-bit).
 * @Note TIM2 doit être configuré avec un prescaler de 83 pour une résolution de 1us.
 */
struct SystemClock {
    
    static void init() {
        __HAL_RCC_TIM2_CLK_ENABLE();
        
        TIM2->PSC = 83; // 84MHz / 84 = 1MHz (1us tick)
        TIM2->ARR = 0xFFFFFFFF; // 32-bit max
        TIM2->CR1 |= TIM_CR1_CEN;
    }

    static Microseconds now_us() {
        return Microseconds(static_cast<uint64_t>(TIM2->CNT));
    }
    
    static Milliseconds now_ms() {
        return Milliseconds(static_cast<uint64_t>(TIM2->CNT / 1000));
    }
    
    static Microseconds elapsed_us(Microseconds from) {
        uint32_t current = TIM2->CNT;
        return Microseconds(static_cast<uint64_t>(current - static_cast<uint32_t>(from.count)));
    }
};

template<typename T>
concept ISystemClock = requires(T t, Microseconds from) {
    { t.now_us() } -> std::same_as<Microseconds>;
    { t.now_ms() } -> std::same_as<Milliseconds>;
    { t.elapsed_us(from) } -> std::same_as<Microseconds>;
};

static_assert(ISystemClock<SystemClock>, "SystemClock does not satisfy ISystemClock concept");

} // namespace hal
