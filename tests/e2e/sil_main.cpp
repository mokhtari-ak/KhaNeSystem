#include <iostream>
#include <chrono>
#include <thread>
#include "BusMessages.hpp"
#include "EventBus.hpp"

// Mock Flight Controller pour Simulation
class SimulationInterface {
public:
    void send_to_simulator(const bus::ActuatorCmd& cmd) {
        // Envoi via UDP/TCP vers AirSim
    }
    
    bus::SensorFrame receive_from_simulator() {
        // Réception via UDP/TCP
        return bus::SensorFrame{};
    }
};

int main() {
    std::cout << "Starting SIL Simulation..." << std::endl;
    SimulationInterface sim;
    
    // Boucle de simulation 400Hz
    while(true) {
        auto sensor_data = sim.receive_from_simulator();
        // Logique de fusion + contrôle
        // ...
        
        bus::ActuatorCmd cmd;
        sim.send_to_simulator(cmd);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    return 0;
}
