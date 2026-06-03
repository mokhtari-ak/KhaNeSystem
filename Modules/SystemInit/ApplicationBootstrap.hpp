#pragma once

#include "SensorIntegration.hpp"
#include "EkfTask.hpp"
#include "FlightControlTask.hpp"
#include "SafetyTask.hpp"
#include "NavigationTask.hpp"
#include "RtosAbstract.hpp"
#include "HalTypes.hpp"

#ifdef USE_THREADX
    #include "ThreadXWrapper.hpp"
    template<size_t S> using RtosTask = rtos::threadx::Task<S>;
#else
    #include "FreeRtosWrapper.hpp"
    template<size_t S> using RtosTask = rtos::freertos::Task<S>;
#endif

namespace task_fusion = modules::sensor_fusion;
namespace task_flight = modules::flight;
namespace task_nav = modules::navigation;

#ifndef SIL_TARGET
    extern "C" IWDG_HandleTypeDef hiwdg;
#endif

namespace system_init {

enum class SystemState {
    PRE_INIT,
    INIT,
    SELF_TEST,
    READY,
    FAULT
};

class ApplicationBootstrap {
public:
    static ::hal::Result<void> init() noexcept {
        current_state_ = SystemState::PRE_INIT;
        
        // INIT Phase
        auto res = perform_init();
        if (!res) {
            current_state_ = SystemState::FAULT;
            return res;
        }
        current_state_ = SystemState::INIT;

        // SELF_TEST Phase
        res = perform_self_test();
        if (!res) {
            current_state_ = SystemState::FAULT;
            return res;
        }
        current_state_ = SystemState::SELF_TEST;

        // READY Phase
        start_tasks();
        current_state_ = SystemState::READY;
        
        return {};
    }

private:
    static ::hal::Result<void> perform_init() noexcept {
        init_sensors();
        return {};
    }

    static ::hal::Result<void> perform_self_test() noexcept {
        return {};
    }

    static void start_tasks() noexcept {
        // Tâches utilisant l'alias agnostique RtosTask
        static RtosTask<2048> sensor_task("Sensor", [](void*){ while(true) { sensor_task_loop(); } }, nullptr, 7);
        static RtosTask<4096> ekf_task("EKF", [](void* p){ static_cast<task_fusion::EkfTask*>(p)->run(); }, &ekf_instance, 6);
        static RtosTask<3072> flight_control_task("FlightCtrl", [](void* p){ static_cast<task_flight::FlightControlTask*>(p)->run(); }, &flight_ctrl_instance, 6);
        static RtosTask<2048> safety_task("Safety", [](void* p){ static_cast<::modules::safety::SafetyTask*>(p)->run(); }, &safety_instance, 7);
        static RtosTask<3072> nav_task("Nav", [](void* p){ static_cast<task_nav::NavigationTask*>(p)->run(); }, &nav_instance, 5);

        sensor_task.start();
        ekf_task.start();
        flight_control_task.start();
        safety_task.start();
        nav_task.start();
    }

    inline static SystemState current_state_ = SystemState::PRE_INIT;
    
    // Instances globales des bus (complétant celles de SensorIntegration.hpp)
    inline static bus::EventBus<bus::GnssFrame> gnss_bus;
    inline static bus::EventBus<bus::StateVector> state_bus;
    inline static bus::EventBus<bus::RcFrame> rc_bus;
    inline static bus::EventBus<bus::ActuatorCmd> output_bus_global;
    inline static bus::EventBus<bus::FaultEvent> fault_bus_global;
    inline static bus::EventBus<bus::NavGuidance> nav_bus_global;

    // Instance Watchdog (Mock ou Réelle selon la cible)
    #ifdef SIL_TARGET
        // Mock ou Simulation
    #else
        inline static ::hal::WatchdogDriver watchdog_instance{&hiwdg};
    #endif

    // Instances des tâches avec leurs dépendances réelles
    inline static task_fusion::EkfTask ekf_instance{sensor_bus, gnss_bus, state_bus};
    inline static task_flight::FlightControlTask flight_ctrl_instance{state_bus, rc_bus, output_bus_global};
    #ifdef SIL_TARGET
        // SafetyTask sans watchdog réel pour la simulation ou avec mock
    #else
        inline static ::modules::safety::SafetyTask safety_instance{fault_bus_global, watchdog_instance};
    #endif
    inline static task_nav::NavigationTask nav_instance{state_bus, nav_bus_global, fault_bus_global};
};

} // namespace system_init
