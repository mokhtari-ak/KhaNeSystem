#include <gtest/gtest.h>
#include "MockSimulationBridge.hpp"
#include "FlightControlTask.hpp"
#include "EventBus.hpp"

using namespace modules::flight;

class SILAcceptanceTest : public ::testing::Test {
protected:
    hal::MockSimulationBridge mock_bridge;
    bus::EventBus<bus::StateVector> state_bus;
    bus::EventBus<bus::RcFrame> rc_bus;
    bus::EventBus<bus::ActuatorCmd> actuator_bus;
};

TEST_F(SILAcceptanceTest, PontMavLinkInitialisation) {
    mock_bridge.set_connected(true);
    EXPECT_TRUE(mock_bridge.is_connected());
}

TEST_F(SILAcceptanceTest, VolStationnaire10s) {
    FlightControlTask task(state_bus, rc_bus, actuator_bus, &mock_bridge);
    
    // Simuler un état de vol stable
    // ... (Logique de simulation du temps et des entrées)
    
    SUCCEED(); // Remplacer par la vérification réelle des commandes moteurs
}

TEST_F(SILAcceptanceTest, GestionFailsafeErreurEKF) {
    FlightControlTask task(state_bus, rc_bus, actuator_bus, &mock_bridge);
    
    // Injecter une erreur et vérifier que le failsafe est déclenché
    // ...
    
    SUCCEED(); // Remplacer par la vérification réelle de l'état Failsafe
}
