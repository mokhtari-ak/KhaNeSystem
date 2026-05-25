#pragma once

#include "BusMessages.hpp"
#include "EventBus.hpp"
#include <array>

namespace nav {

struct Waypoint {
    double lat, lon;
    float alt;
};

class NavigationTask {
public:
    explicit NavigationTask(bus::EventBus<bus::NavGuidance>& guidance_bus)
        : guidance_bus_(guidance_bus) {}

    void update(const bus::StateVector& state) noexcept {
        // 1. Mise à jour position véhicule
        // 2. Calcul vecteur vers waypoint actif
        // 3. Vérification Geofencing (Cylindre)
        
        bus::NavGuidance guidance{
            .header = { .version = 1, .timestamp = hal::Microseconds(0) },
            .target_heading = 0.0f,
            .target_alt = 100.0f,
            .target_speed = 15.0f,
            .xte = 0.0f
        };
        
        guidance_bus_.publish(guidance);
    }

private:
    bus::EventBus<bus::NavGuidance>& guidance_bus_;
    std::array<Waypoint, 256> mission_{};
    size_t current_waypoint_ = 0;
};

} // namespace nav
