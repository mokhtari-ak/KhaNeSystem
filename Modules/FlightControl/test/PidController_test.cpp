#include "PidController.hpp"
#include <cassert>
#include <cmath>

using namespace control;

void test_p_only() {
    PidController<float, Axis::Roll> pid;
    PidController<float, Axis::Roll>::Gains gains{1.0f, 0.0f, 0.0f, 0.0f, 10.0f, 0.01f};
    pid.set_gains(gains);
    
    // Erreur de 5.0, P=1.0 -> Sortie 5.0
    float output = pid.update(10.0f, 5.0f, hal::Microseconds{1000000}); // 1s
    assert(std::abs(output - 5.0f) < 0.001f);
}

void test_i_antiwindup() {
    PidController<float, Axis::Roll> pid;
    // I=1.0, Limit=2.0
    PidController<float, Axis::Roll>::Gains gains{0.0f, 1.0f, 0.0f, 2.0f, 10.0f, 0.01f};
    pid.set_gains(gains);
    
    // 3 secondes d'erreur de 1.0 -> I devrait atteindre 2.0 (la limite)
    pid.update(1.0f, 0.0f, hal::Microseconds{1000000});
    pid.update(1.0f, 0.0f, hal::Microseconds{1000000});
    float output = pid.update(1.0f, 0.0f, hal::Microseconds{1000000});
    
    assert(std::abs(output - 2.0f) < 0.001f);
}

int main() {
    test_p_only();
    test_i_antiwindup();
    return 0;
}
