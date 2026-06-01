#pragma once

#include "RtosAbstract.hpp"
#include "BusMessages.hpp"
#include "EventBus.hpp"
#include "SensorFusion.hpp"

namespace modules::sensor_fusion {

class EkfTask {
public:
    EkfTask(bus::EventBus<bus::SensorFrame>& sensor_bus, 
            bus::EventBus<bus::GnssFrame>& gnss_bus,
            bus::EventBus<bus::StateVector>& output_bus) 
        : sensor_bus_(sensor_bus), gnss_bus_(gnss_bus), output_bus_(output_bus) {}

    void init() {
        // Souscription aux flux d'entrées
    }

    void run() {
        while (true) {
            // Lecture des données du bus
            // Exécution du cycle EKF (Predict/Update)
            // Publication de l'état estimé
            
            // 250 Hz (4ms)
            vTaskDelay(pdMS_TO_TICKS(4));
        }
    }

private:
    bus::EventBus<bus::SensorFrame>& sensor_bus_;
    bus::EventBus<bus::GnssFrame>& gnss_bus_;
    bus::EventBus<bus::StateVector>& output_bus_;
    EkfWrapper ekf_wrapper_;
};

} // namespace modules::sensor_fusion
