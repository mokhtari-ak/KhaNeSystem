#pragma once

#include "RtosAbstract.hpp"
#include "BusMessages.hpp"
#include "PidController.hpp"
#include "EventBus.hpp"

namespace control {

template<typename DroneType>
class FlightControlTask {
public:
    explicit FlightControlTask(bus::EventBus<bus::ActuatorCmd>& output_bus)
        : output_bus_(output_bus) {}

    // Boucle de contrôle à 400 Hz (appelée par la tâche RTOS)
    void update(const bus::StateVector& state, const bus::RcFrame& rc) noexcept {
        // 1. Calcul des consignes (ex: Yaw -> YawRate)
        
        // 2. Mise à jour PID (utilisation de std::array pour les axes)
        float roll_out = pid_roll_.update(rc.channels[0], state.attitude.x(), hal::Microseconds(2500));
        float pitch_out = pid_pitch_.update(rc.channels[1], state.attitude.y(), hal::Microseconds(2500));
        float yaw_out = pid_yaw_.update(rc.channels[2], state.attitude.z(), hal::Microseconds(2500));

        // 3. Construction et publication de la commande actionneur
        bus::ActuatorCmd cmd{
            .header = { .version = 1, .timestamp = hal::Microseconds(0) },
            .channels = { roll_out, pitch_out, yaw_out, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
        };

        output_bus_.publish(cmd);
    }

private:
    bus::EventBus<bus::ActuatorCmd>& output_bus_;
    PidController<DroneType, Axis::Roll> pid_roll_;
    PidController<DroneType, Axis::Pitch> pid_pitch_;
    PidController<DroneType, Axis::Yaw> pid_yaw_;
};

} // namespace control
