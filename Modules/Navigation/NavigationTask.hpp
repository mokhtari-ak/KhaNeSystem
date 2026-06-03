#pragma once

#include "RtosAbstract.hpp"
#include "BusMessages.hpp"
#include "EventBus.hpp"
#include "SystemClock.hpp"
#include <array>

namespace modules::navigation {

struct Waypoint {
    float lat, lon; // Utilisé comme coordonnées locales en mètres pour simplifier
    float alt;
};

class NavigationTask {
public:
    NavigationTask(bus::EventBus<bus::StateVector>& state_bus,
                   bus::EventBus<bus::NavGuidance>& guidance_bus,
                   bus::EventBus<bus::FaultEvent>& fault_bus)
        : state_bus_(state_bus), guidance_bus_(guidance_bus), fault_bus_(fault_bus) {}

    void init() {
        current_waypoint_ = 0;
    }

    void run() {
        TickType_t xLastWakeTime = xTaskGetTickCount();
        const TickType_t xFrequency = pdMS_TO_TICKS(20); // 50Hz = 20ms

        while (true) {
            // ... (lecture état)
            bus::StateVector current_state; 

            // 2. Vérifier Geofencing (sécurité)
            if (check_geofence(current_state)) {
                bus::FaultEvent fault{
                    .header = { .version = 1, .msg_id = bus::MessageType::FaultEvent, .timestamp = hal::Microseconds(0) },
                    .fault_mask = 0x01, // GEOFENCE_VIOLATION
                    .source_id = 0
                };
                fault_bus_.publish(fault);
            }
            // ... (suite)

            // 3. Calculer l'erreur de navigation vers le waypoint cible
            const Waypoint& target = mission_[current_waypoint_];
            float dx = target.lat - current_state.position.x(); // Exemple mapping
            float dy = target.lon - current_state.position.y();
            float dist_sq = dx*dx + dy*dy;

            // 4. Passage automatique au waypoint
            if (dist_sq < 2.0f * 2.0f) { // Rayon d'acceptation de 2m
                if (current_waypoint_ < 255) current_waypoint_++;
            }

            // 5. Publier NavGuidance
            bus::NavGuidance guidance{
                .header = { .version = 1, .msg_id = bus::MessageType::NavGuidance, .timestamp = hal::Microseconds(0) },
                .target_heading = std::atan2(dy, dx),
                .target_alt = target.alt,
                .target_speed = 10.0f,
                .xte = 0.0f // Simplifié
            };
            guidance_bus_.publish(guidance);

            vTaskDelayUntil(&xLastWakeTime, xFrequency);
        }
    }

private:
    bool check_geofence(const bus::StateVector& state) {
        // Distance au point d'origine (0,0) > 500m
        return (state.position.x()*state.position.x() + state.position.y()*state.position.y()) > (500.0f * 500.0f);
    }

private:
    bus::EventBus<bus::StateVector>& state_bus_;
    bus::EventBus<bus::NavGuidance>& guidance_bus_;
    bus::EventBus<bus::FaultEvent>& fault_bus_;
    
    std::array<Waypoint, 256> mission_{};
    size_t current_waypoint_ = 0;
};

} // namespace modules::navigation
