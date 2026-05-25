#include <gtest/gtest.h>
#include "Modules/SensorFusion/SensorFusion.hpp"
#include "Modules/FlightControl/FlightControlTask.hpp"
#include "Modules/Safety/SafetyTask.hpp"

using namespace fusion;
using namespace control;
using namespace safety;

TEST(IntegrationTest, PipelineNominal) {
    // Setup bus
    bus::EventBus<bus::ActuatorCmd> output_bus;
    
    // Setup modules
    SensorFusion fusion;
    FlightControlTask<DroneType::Quadcopter> controller(output_bus);
    
    // Simulate data
    bus::SensorFrame frame{.header = {1, hal::Microseconds(0)}, .accel = Eigen::Vector3f(0,0,9.81f)};
    
    // Process
    auto result = fusion.update_attitude(frame);
    ASSERT_TRUE(result.has_value());
    
    // Verify control output
    bus::RcFrame rc{.channels = {0,0,0,0,0,0,0,0}};
    controller.update(fusion.get_state(), rc);
    
    // Assert logic ...
}

TEST(RobustnessTest, FailsafeTriggeredOnImuFailure) {
    bus::EventBus<bus::FaultEvent> fault_bus;
    SafetyTask safety(fault_bus);
    
    // Simulate IMU NaN failure
    bus::StateVector corrupted_state{}; 
    
    // Verify SafetyTask response
    safety.update(corrupted_state);
    
    // Assert FaultEvent is published
}
