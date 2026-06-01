#pragma once

#include "RtosAbstract.hpp"
#include "BusMessages.hpp"
#include "EventBus.hpp"
#include "WatchdogDriver.hpp"
#include <bitset>
#include <algorithm>

namespace safety {

enum class FailsafeLevel { None, Warning, Critical, Emergency, Lockout };

// Identifiants des tâches critiques pour le checkpointing
enum class TaskId : uint32_t {
    ImuTask = 1 << 0,
    EkfTask = 1 << 1,
    FlightControl = 1 << 2,
    Navigation = 1 << 3,
    MandatoryTasks = ImuTask | EkfTask | FlightControl
};

class SafetyTask {
public:
    explicit SafetyTask(bus::EventBus<bus::FaultEvent>& fault_bus, hal::IWatchdog& watchdog) 
        : fault_bus_(fault_bus), watchdog_(watchdog) {}

    void init() {
        current_level_ = FailsafeLevel::None;
        fault_mask_.reset();
        health_checkpoints_ = 0;
    }

    // Chaque tâche critique doit appeler cette méthode à chaque cycle
    void report_health(TaskId task) noexcept {
        health_checkpoints_ |= static_cast<uint32_t>(task);
    }

    void run() {
        TickType_t xLastWakeTime = xTaskGetTickCount();
        const TickType_t xFrequency = pdMS_TO_TICKS(2); // 400Hz nominal

        while (true) {
            // 1. Mise à jour de l'état global des fautes (via subscription au bus)
            
            // 2. Machine à états Failsafe
            update_failsafe_level();
            
            // 3. Rafraîchissement Watchdog (Checkpointing + Failsafe Level)
            const uint32_t mandatory = static_cast<uint32_t>(TaskId::MandatoryTasks);
            bool all_healthy = (health_checkpoints_ & mandatory) == mandatory;

            if (all_healthy && current_level_ < FailsafeLevel::Critical) {
                watchdog_.refresh();
            }
            
            // Reset des checkpoints pour le prochain cycle
            health_checkpoints_ = 0;
            
            vTaskDelayUntil(&xLastWakeTime, xFrequency);
        }
    }

private:
    void update_failsafe_level() {
        if (fault_mask_.any()) {
            current_level_ = FailsafeLevel::Critical;
        } else {
            current_level_ = FailsafeLevel::None;
        }
    }

    bus::EventBus<bus::FaultEvent>& fault_bus_;
    hal::IWatchdog& watchdog_;
    
    FailsafeLevel current_level_;
    std::bitset<32> fault_mask_;
    uint32_t health_checkpoints_ = 0;
};

} // namespace safety
