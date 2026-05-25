#pragma once

#include "SensorIntegration.hpp"
#include "EkfTask.hpp"
#include "FlightControlTask.hpp"
#include "SafetyTask.hpp"
#include "NavigationTask.hpp"
#include "RtosAbstract.hpp"

#ifdef USE_THREADX
    #include "ThreadXWrapper.hpp"
    template<size_t S> using RtosTask = rtos::threadx::Task<S>;
#else
    #include "FreeRtosWrapper.hpp"
    template<size_t S> using RtosTask = rtos::freertos::Task<S>;
#endif

namespace system_init {

class ApplicationBootstrap {
public:
    static void init() noexcept {
        init_sensors();

        // Tâches utilisant l'alias agnostique RtosTask
        static RtosTask<2048> sensor_task("Sensor", [](void*){ while(true) { sensor_task_loop(); } }, nullptr, 7);
        static RtosTask<4096> ekf_task("EKF", [](void* p){ static_cast<fusion::EkfTask*>(p)->run(); }, &ekf_instance, 6);
        static RtosTask<3072> flight_control_task("FlightCtrl", [](void* p){ /* ... */ }, &flight_ctrl_instance, 6);
        static RtosTask<2048> safety_task("Safety", [](void* p){ /* ... */ }, &safety_instance, 7);
        static RtosTask<3072> nav_task("Nav", [](void* p){ /* ... */ }, &nav_instance, 5);

        sensor_task.start();
        ekf_task.start();
        flight_control_task.start();
        safety_task.start();
        nav_task.start();
    }

private:
    inline static fusion::EkfTask ekf_instance{ekf_queue, sensor_bus};
    inline static control::FlightControlTask<DroneType::Quadcopter> flight_ctrl_instance{output_bus_global};
    inline static safety::SafetyTask safety_instance{fault_bus_global};
    inline static nav::NavigationTask nav_instance{nav_bus_global};

    inline static bus::EventBus<bus::ActuatorCmd> output_bus_global;
    inline static bus::EventBus<bus::FaultEvent> fault_bus_global;
    inline static bus::EventBus<bus::NavGuidance> nav_bus_global;
};

} // namespace system_init
