#include <iostream>
#include <cassert>
#include "../Modules/FlightControl/PidController.hpp"

// Mock de HalTypes.hpp nécessaire pour PidController.hpp
namespace hal {
    struct Microseconds { uint64_t count; };
}

int main() {
    control::PidController<void, control::Axis::Roll> pid;
    
    control::PidController<void, control::Axis::Roll>::Gains gains;
    gains.p = 1.0f;
    gains.i = 0.1f;
    gains.d = 0.05f;
    gains.i_limit = 10.0f;
    gains.output_limit = 100.0f;
    gains.d_filter_tau = 0.01f;
    
    pid.set_gains(gains);
    
    // Test simple : erreur positive -> sortie positive
    float output = pid.update(10.0f, 0.0f, hal::Microseconds{10000}); // 10ms
    
    std::cout << "PID output: " << output << std::endl;
    assert(output > 0.0f);
    
    std::cout << "Test passed!" << std::endl;
    return 0;
}
