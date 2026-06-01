#pragma once

#include "RtosAbstract.hpp"
#include "BusMessages.hpp"
#include "EventBus.hpp"
#include "PidController.hpp"
#include "SystemClock.hpp"

namespace modules::flight {

class FlightControlTask {
public:
    FlightControlTask(bus::EventBus<bus::StateVector>& state_bus,
                      bus::EventBus<bus::RcFrame>& rc_bus,
                      bus::EventBus<bus::ActuatorCmd>& actuator_bus)
        : state_bus_(state_bus), rc_bus_(rc_bus), actuator_bus_(actuator_bus) {}

    void init() {
        // Initialisation des gains PID
        // Abonnement aux bus
    }

    void run() {
        TickType_t xLastWakeTime = xTaskGetTickCount();
        const TickType_t xFrequency = pdMS_TO_TICKS(2); // 400Hz nominal (approx 2.5ms), sécurisé à 2ms ou 3ms selon tickrate

        bus::StateVector state;
        bus::RcFrame rc;
        hal::Microseconds last_rc_time(0);

        while (true) {
            // 1. Lire bus (non-bloquant)
            // state_bus_.pop(state);
            // rc_bus_.pop(rc);

            // 2. Failsafe (RC Timeout 100ms)
            if ((SystemClock::get_time() - last_rc_time).count > 100000) {
                // Trigger Failsafe (ex: publier commande zéro)
            }

            // 3. Calcul PID
            hal::Microseconds dt(2500);
            float roll_out = pid_roll_.update(rc.channels[0], state.attitude.x(), dt);
            float pitch_out = pid_pitch_.update(rc.channels[1], state.attitude.y(), dt);
            float yaw_out = pid_yaw_.update(rc.channels[2], state.attitude.z(), dt);
            float throttle = rc.channels[3];

            // 4. Mixage (QuadX simplifié)
            bus::ActuatorCmd cmd{
                .header = { .version = 1, .msg_id = bus::MessageType::ActuatorCmd, .timestamp = SystemClock::get_time() },
                .channels = { 
                    throttle + roll_out + pitch_out + yaw_out, // M1
                    throttle - roll_out + pitch_out - yaw_out, // M2
                    throttle + roll_out - pitch_out - yaw_out, // M3
                    throttle - roll_out - pitch_out + yaw_out, // M4
                    0, 0, 0, 0 
                }
            };
            actuator_bus_.publish(cmd);

            vTaskDelayUntil(&xLastWakeTime, xFrequency);
        }
    }

private:
    bus::EventBus<bus::StateVector>& state_bus_;
    bus::EventBus<bus::RcFrame>& rc_bus_;
    bus::EventBus<bus::ActuatorCmd>& actuator_bus_;
    control::PidController<float, control::Axis::Roll> pid_roll_;
    control::PidController<float, control::Axis::Pitch> pid_pitch_;
    control::PidController<float, control::Axis::Yaw> pid_yaw_;
};

} // namespace modules::flight
