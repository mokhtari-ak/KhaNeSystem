#pragma once

#include "BusMessages.hpp"
#include "PwmDriver.hpp"
#include "PwmConfigPolicy.hpp"
#include <algorithm>

namespace modules::flight {

class PropulsionController {
public:
    PropulsionController() = default;

    // Convertit les commandes normalisées [-1.0, 1.0] ou [0.0, 1.0] en valeur PWM
    void apply_actuator_cmd(const bus::ActuatorCmd& cmd) {
        for (size_t i = 0; i < 4; ++i) {
            float thrust = std::clamp(cmd.channels[i], 0.0f, 1.0f);
            uint32_t pwm_value = static_cast<uint32_t>(thrust * 1000.0f + 1000.0f); // 1000us - 2000us
            
            // Appliquer sur le PWM correspondant
            // pwm_driver_.setDutyCycle(..., ..., pwm_value);
        }
    }

private:
    hal::HalPwmDriver pwm_driver_;
};

} // namespace modules::flight
