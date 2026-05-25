## Epic 1: Socle Matériel & BSP
### Story 1.1: Setup STM32F4 Project structure (CubeMX + CMake)
### Story 1.2: Configuration des toolchains et flags compilateur C++20
### Story 1.3: Implémentation de la couche STM32F4HAL (Wrappers)

## Epic 2: RTOS & Communication
### Story 2.1: Implémentation des abstractions RTOS (FreeRTOS/ThreadX)
### Story 2.2: Développement de l'Internal Event Bus typé
### Story 2.3: Implémentation des types de messages Bus (SensorFrame, StateVector, etc.)

## Epic 3: Sensor Fusion & Drivers
### Story 3.1: Drivers Capteurs (Imu, Baro, Gnss, Magneto, Airspeed)
### Story 3.2: Intégration bibliothèque KalmanFilter
### Story 3.3: Module SensorFusion (EKF Wrapper)

## Epic 4: Flight Control & Navigation
### Story 4.1: Implémentation des contrôleurs PID déterministes
### Story 4.2: FlightControlTask (Boucle 400Hz)
### Story 4.3: NavigationTask (Waypoints, Geofencing)

## Epic 5: Sécurité & Supervision
### Story 5.1: SafetyTask (FaultBus, Watchdog, Failsafe)
### Story 5.2: ConfigurationManager & Persistance Flash
### Story 5.3: Mise en place CI/CD et vérification mémoire
