#pragma once

#include "RtosAbstract.hpp"
#include "BusMessages.hpp"
#include "EventBus.hpp"
#include "PidController.hpp"
#include "SystemClock.hpp"
#include "ISimulationBridge.hpp"

namespace modules::flight {

class FlightControlTask {
public:
    FlightControlTask(bus::EventBus<bus::StateVector>& state_bus,
                      bus::EventBus<bus::RcFrame>& rc_bus,
                      bus::EventBus<bus::ActuatorCmd>& actuator_bus,
                      ::hal::ISimulationBridge* sim_bridge = nullptr)
        : state_bus_(state_bus), rc_bus_(rc_bus), actuator_bus_(actuator_bus), sim_bridge_(sim_bridge) {}

    void init() {
        // Initialisation des gains PID
        // Abonnement aux bus
    }

    void run() {
        TickType_t xLastWakeTime = xTaskGetTickCount();
        const TickType_t xFrequency = pdMS_TO_TICKS(2); // 400Hz nominal

        bus::StateVector state;
        bus::RcFrame rc;
        ::hal::Microseconds last_rc_time(0);

        while (true) {
            // 2. Failsafe (RC Timeout 100ms)
            if ((::hal::SystemClock::now_us().count - last_rc_time.count) > 100000) {
                // Trigger Failsafe
            }

            // 3. Calcul PID
            ::hal::Microseconds dt(2500);
            float roll_out = pid_roll_.update(rc.channels[0], state.position.x(), dt); // Exemple position.x pour la mesure
            float pitch_out = pid_pitch_.update(rc.channels[1], state.position.y(), dt);
            float yaw_out = pid_yaw_.update(rc.channels[2], state.position.z(), dt);
            float throttle = rc.channels[3];

            // 4. Mixage (QuadX simplifié)
            bus::ActuatorCmd cmd{
                .header = { .version = 1, .msg_id = bus::MessageType::ActuatorCmd, .timestamp = ::hal::SystemClock::now_us() },
                .channels = { 
                    throttle + roll_out + pitch_out + yaw_out, // M1
                    throttle - roll_out + pitch_out - yaw_out, // M2
                    throttle + roll_out - pitch_out - yaw_out, // M3
                    throttle - roll_out - pitch_out + yaw_out, // M4
                    0, 0, 0, 0 
                }
            };
            actuator_bus_.publish(cmd);

            // Simulation bridge interaction if connected
            if (sim_bridge_ && sim_bridge_->is_connected()) {
                // send commands to simulator
            }

            vTaskDelayUntil(&xLastWakeTime, xFrequency);
        }
    }

private:
    bus::EventBus<bus::StateVector>& state_bus_;
    bus::EventBus<bus::RcFrame>& rc_bus_;
    bus::EventBus<bus::ActuatorCmd>& actuator_bus_;
    ::hal::ISimulationBridge* sim_bridge_;
    // Utilisation d'un type fictif 'void' pour DroneType si non spécifié, ou un enum correct.
    // Selon PidController.hpp: template<typename DroneType, Axis axis>
    control::PidController<void, control::Axis::Roll> pid_roll_;
    control::PidController<void, control::Axis::Pitch> pid_pitch_;
    control::PidController<void, control::Axis::Yaw> pid_yaw_;
};

} // namespace modules::flight
