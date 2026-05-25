#pragma once

#include "RtosAbstract.hpp"
#include "SensorFusion.hpp"
#include "EventBus.hpp"
#include "BusMessages.hpp"

namespace fusion {

// Tâche de traitement EKF (250 Hz)
class EkfTask {
public:
    explicit EkfTask(rtos::IQueue<bus::SensorFrame, 8>& input_queue, 
                     bus::EventBus<bus::StateVector>& output_bus)
        : input_queue_(input_queue), output_bus_(output_bus) {}

    void run() noexcept {
        // Boucle de traitement temps-réel
        while (true) {
            auto frame = input_queue_.pop(hal::Microseconds(5000));
            if (frame) {
                // 1. Mise à jour EKF
                auto result = fusion_.update_attitude(*frame);
                
                // 2. Publication du nouvel état
                if (result) {
                    output_bus_.publish(fusion_.get_state());
                }
            }
        }
    }

private:
    rtos::IQueue<bus::SensorFrame, 8>& input_queue_;
    bus::EventBus<bus::StateVector>& output_bus_;
    SensorFusion fusion_;
};

} // namespace fusion
