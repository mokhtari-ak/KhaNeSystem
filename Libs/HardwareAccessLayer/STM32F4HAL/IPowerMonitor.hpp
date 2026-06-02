#pragma once

#include "HalTypes.hpp"
#include <cstdint>

namespace hal {

/**
 * @brief Interface pour le moniteur de puissance (tension/courant).
 */
class IPowerMonitor {
public:
    virtual ~IPowerMonitor() = default;

    struct PowerData {
        float voltage; // Volts
        float current; // Amps
    };

    virtual Result<PowerData> get_data() noexcept = 0;
    virtual bool is_ready() const noexcept = 0;
};

} // namespace hal
