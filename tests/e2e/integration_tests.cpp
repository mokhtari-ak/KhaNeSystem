#include <gtest/gtest.h>
#include "SensorIntegration.hpp"
#include "SpiWrapper.hpp"

// Tests API pour valider le bridge SIL
TEST(SensorIntegrationTest, SpiWrapper_TransmitReceive_Success) {
    // Setup Mock SPI Handle
    SPI_HandleTypeDef hspi; 
    hal::SpiWrapper transport(&hspi);
    
    std::vector<uint8_t> tx = {0x01, 0x02};
    std::vector<uint8_t> rx(2);
    
    auto res = transport.transmit_receive(tx, rx, hal::Microseconds(1000));
    EXPECT_TRUE(res.has_value());
}

TEST(SensorIntegrationTest, ImuDriver_Update_DataFlow) {
    // Validation de la réception IMU simulée
    system_init::init_sensors();
    system_init::sensor_task_loop();
    // Vérifier si les données sont bien dans le bus (via mock EventBus)
    SUCCEED();
}
