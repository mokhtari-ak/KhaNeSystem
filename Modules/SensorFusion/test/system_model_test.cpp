#include <SensorFusion.hpp>
#include <iostream>

using namespace modules::sensor_fusion;

int main() {
    DroneSystemModel model;
    StateVector x;
    x.setZero();
    Eigen::Matrix<float, 6, 1> u;
    u.setZero();
    
    // Appel à f pour tester l'interface
    StateVector x_new = model.f(x, u);
    
    // Appel à updateJacobians pour tester l'interface
    model.updateJacobians(x, u);
    
    std::cout << "Test successful" << std::endl;
    return 0;
}
