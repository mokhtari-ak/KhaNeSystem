#pragma once

#include "BusMessages.hpp"
#include "ISimulationBridge.hpp"
#include "EventBus.hpp"

namespace modules::test {

/**
 * @Brief Interface HITL (Hardware-in-the-Loop)
 * Permet au STM32F4 d'échanger des frames avec un simulateur externe via abstraction.
 */
class HitlInterface {
public:
    HitlInterface(bus::EventBus<bus::SensorFrame>& sensor_bus, hal::ISimulationBridge& sim_bridge)
        : sensor_bus_(sensor_bus), sim_bridge_(sim_bridge) {}

    // Traite les paquets entrants du simulateur
    void process_incoming() {
        uint8_t buffer[128];
        auto res = sim_bridge_.receive(buffer);
        // Décodage et publication sur bus
    }

    // Envoie les commandes actuateurs au simulateur
    void send_actuators(const bus::ActuatorCmd& cmd) {
        uint8_t buffer[64];
        // Sérialisation
        sim_bridge_.send(buffer);
    }

private:
    bus::EventBus<bus::SensorFrame>& sensor_bus_;
    hal::ISimulationBridge& sim_bridge_;
};

} // namespace modules::test
