#pragma once

#include "RtosAbstract.hpp"
#include "BusMessages.hpp"
#include "EventBus.hpp"
#include <bitset>

namespace safety {

enum class FailsafeLevel { None, Warning, Critical, Emergency, Lockout };

class SafetyTask {
public:
    explicit SafetyTask(bus::EventBus<bus::FaultEvent>& fault_bus) 
        : fault_bus_(fault_bus) {}

    void update(const bus::StateVector& state) noexcept {
        uint32_t faults = check_system_integrity(state);
        
        if (faults != 0) {
            handle_faults(faults);
        }
        
        kick_watchdog();
    }

private:
    uint32_t check_system_integrity(const bus::StateVector& state) noexcept {
        uint32_t faults = 0;
        // Validation EKF, CPU load, etc.
        return faults;
    }

    void handle_faults(uint32_t faults) noexcept {
        // Logique de réponse Failsafe
    }

    void kick_watchdog() noexcept {
        // Kick IWDG
    }

    bus::EventBus<bus::FaultEvent>& fault_bus_;
};

} // namespace safety
