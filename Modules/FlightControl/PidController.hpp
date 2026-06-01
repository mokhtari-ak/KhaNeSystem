#pragma once

#include "HalTypes.hpp"
#include <algorithm>

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
        float output_limit;
        float d_filter_tau; // Constante de temps pour le filtre passe-bas (secondes)
    };

    PidController() = default;

    // Calcul PID : D sur la mesure, avec filtrage passe-bas
    float update(float setpoint, float measured, hal::Microseconds dt) noexcept {
        float delta_t = static_cast<float>(dt.count) / 1000000.0f; // µs -> s
        if (delta_t <= 0.0f) return 0.0f;

        float error = setpoint - measured;

        // P
        float p_out = gains_.p * error;

        // I avec anti-windup
        integrator_ += error * delta_t;
        integrator_ = std::clamp(integrator_, -gains_.i_limit, gains_.i_limit);
        float i_out = gains_.i * integrator_;

        // D sur la mesure (D-kick avoidance) avec filtre passe-bas
        float derivative = (measured - prev_measured_) / delta_t;
        
        // Filtre passe-bas simple : y[n] = alpha * x[n] + (1 - alpha) * y[n-1]
        float alpha = delta_t / (gains_.d_filter_tau + delta_t);
        filtered_derivative_ = alpha * derivative + (1.0f - alpha) * filtered_derivative_;
        
        float d_out = -gains_.d * filtered_derivative_;
        prev_measured_ = measured;

        return std::clamp(p_out + i_out + d_out, -gains_.output_limit, gains_.output_limit);
    }

    void set_gains(const Gains& gains) noexcept {
        gains_ = gains;
    }

    void reset() noexcept {
        integrator_ = 0.0f;
        prev_measured_ = 0.0f;
        filtered_derivative_ = 0.0f;
    }

private:
    Gains gains_{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.01f}; // tau par défaut 10ms
    float integrator_ = 0.0f;
    float prev_measured_ = 0.0f;
    float filtered_derivative_ = 0.0f;
};

} // namespace control
