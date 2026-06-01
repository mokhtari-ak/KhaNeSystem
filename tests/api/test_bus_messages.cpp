#include <iostream>
#include <type_traits>
#include "Modules/InternalEventBus/BusMessages.hpp"

using namespace bus;

int main() {
    static_assert(std::is_trivially_copyable_v<SensorFrame>, "SensorFrame non POD");
    static_assert(std::is_trivially_copyable_v<StateVector>, "StateVector non POD");
    static_assert(sizeof(SensorFrame) <= 128, "SensorFrame trop gros");
    
    std::cout << "Tous les tests bus_messages ont passe avec succes." << std::endl;
    return 0;
}
