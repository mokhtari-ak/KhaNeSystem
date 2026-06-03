#include "SpiWrapper.hpp"
#include <iostream>

int main() {
    // This is a minimal compilation test
    // We don't need a running SPI hardware for this test,
    // just to check if SpiWrapper.hpp compiles with Result type.
    
    // SPI_HandleTypeDef hspi; // Cannot easily instantiate this without HAL
    
    std::cout << "SpiWrapper included successfully." << std::endl;
    return 0;
}
