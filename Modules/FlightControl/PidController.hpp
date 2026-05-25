#pragma once

#include "HalTypes.hpp"
#include <array>

namespace control {

enum class Axis { Roll, Pitch, Yaw, Altitude, Airspeed };

template<typename DroneType, Axis axis>
class PidController {
public:
    struct Gains {
        float p;
        float i;
        float d;
        float i_limit;
    };

    PidController() = default;

    // Calcul PID sans allocation, non-bloquant
    float update(float setpoint, float measured, hal::Microseconds dt) noexcept {
        float error = setpoint - measured;
        
        // Calcul P, I (clamping), D
        // Application anti-windup (clamping)
        
        return output_;
    }

    void set_gains(const Gains& gains) noexcept {
        gains_ = gains;
    }

    void reset() noexcept {
        integrator_ = 0.0f;
    }

private:
    Gains gains_{};
    float integrator_ = 0.0f;
    float output_ = 0.0f;
};

} // namespace control
