# KhaNeSystem

Firmware de vol modulaire, déterministe et hautement sécurisé pour drones (Fixed-Wing & Quadcopter) basé sur **STM32F407VG**.

## Architecture
- **Langage** : C++20 (sous-ensemble embarqué critique).
- **Contraintes** : STM32F407VG (192 KB RAM, 1 MB Flash).
- **Build** : CMake (Toolchain ARM GNU).
- **RTOS** : Abstraction agnostique (FreeRTOS ou ThreadX).

## Directives Techniques
- **Zero-Allocation** : Aucune allocation dynamique (`malloc`, `new`) dans les chemins temps-réel.
- **Sécurité** : Pattern `Result<T, E>` pour la gestion d'erreurs, pas d'exceptions, pas de RTTI.
- **Hardware Abstraction** : Accès matériel via wrappers C++ conformes aux concepts C++20.
- **Sécurité Mémoire** : Aucun buffer DMA en CCM.

## Structure des Modules
- `Core/` : Code de démarrage STM32 (CubeMX).
- `Libs/` : Wrappers HAL (STM32F4HAL) et politiques de drivers.
- `Modules/` : Logique applicative (SensorFusion, FlightControl, Navigation, Safety).
- `Modules/InternalEventBus/` : Communication inter-tâches typée.
- `Modules/ConfigurationManager/` : Persistance Flash avec double-buffer CRC32.

## Build
```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## CI/CD
- Intégration continue via GitHub Actions.
- Analyse statique : `clang-tidy`, `cppcheck`.
- Vérification mémoire : script post-build `check_memory.py` garantissant l'absence de buffers DMA en CCM.

## Tests
- Suite de tests unitaire/intégration basée sur GoogleTest.
- Tests HIL/SIL supportés par injection via mocks (hooks) de drivers.
