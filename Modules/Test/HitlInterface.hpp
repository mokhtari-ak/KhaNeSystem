#pragma once

#include "BusMessages.hpp"
#include "UartDriver.hpp"
#include "EventBus.hpp"

namespace modules::test {

/**
 * @Brief Interface HITL (Hardware-in-the-Loop)
 * Permet au STM32F4 d'échanger des frames avec un simulateur externe via UART.
 */
class HitlInterface {
public:
    HitlInterface(bus::EventBus<bus::SensorFrame>& sensor_bus, int8_t uart_handle)
        : sensor_bus_(sensor_bus), uart_handle_(uart_handle) {}

    // Traite les paquets entrants du simulateur
    void process_incoming() {
        uint8_t buffer[128];
        // hal::HalUartDriver::receive(...)
        // Décodage et publication sur bus
    }

    // Envoie les commandes actuateurs au simulateur
    void send_actuators(const bus::ActuatorCmd& cmd) {
        uint8_t buffer[64];
        // Sérialisation
        hal::HalUartDriver::transmit(uart_handle_, buffer, sizeof(buffer), 100);
    }

private:
    bus::EventBus<bus::SensorFrame>& sensor_bus_;
    int8_t uart_handle_;
};

} // namespace modules::test
