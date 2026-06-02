#pragma once

#include "IPowerMonitor.hpp"
#include "IAdcDriver.hpp"

namespace hal {

/**
 * @brief Implémentation via ADC pour le moniteur de puissance.
 */
class AdcPowerMonitor : public IPowerMonitor {
public:
    explicit AdcPowerMonitor(IAdcDriver& adc) : adc_(adc) {}

    Result<PowerData> get_data() noexcept override {
        // Lecture ADC simulée ou via HAL ADC
        auto v_res = adc_.read_channel(0); // Canal 0: Tension
        auto i_res = adc_.read_channel(1); // Canal 1: Courant

        if (!v_res || !i_res) return std::unexpected(HalError::Error);

        // Conversion brute vers Volts/Amps
        float voltage = static_cast<float>(*v_res) * 0.01f;
        float current = static_cast<float>(*i_res) * 0.001f;
        
        return PowerData{voltage, current};
    }

    bool is_ready() const noexcept override {
        return adc_.is_ready();
    }

private:
    IAdcDriver& adc_;
};

} // namespace hal
