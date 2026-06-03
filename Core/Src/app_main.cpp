#include "app_bridge.h"
#include "../../Modules/SystemInit/ApplicationBootstrap.hpp"

// Point d'entrée C++
void app_main(void) {
    // Lancement de la séquence de démarrage via la machine à états
    auto res = system_init::ApplicationBootstrap::init();
    
    if (!res) {
        // En cas d'erreur au boot, gestion du FAULT
        while(true);
    }
}
