## Epic 1: Socle Matériel & BSP
### Story 1.1: Setup STM32F4 Project structure (CubeMX + CMake)
### Story 1.2: Configuration des toolchains et flags compilateur C++20
### Story 1.3: Implémentation de la couche STM32F4HAL (Wrappers)
### Story 1.4: Implémentation du système de typage fort (Strong Typedefs/Units)

## Epic 2: RTOS & Communication
### Story 2.1: Implémentation des abstractions RTOS (FreeRTOS/ThreadX)
### Story 2.2: Développement de l'Internal Event Bus typé
### Story 2.3: Implémentation des types de messages Bus (SensorFrame, StateVector, etc.)
### Story 2.4: Développement des tâches I/O (RcInputTask, TelemetryTask)
### Story 2.5: Développement du sous-module Storage (Abstraction FS)
### Story 2.6: Développement du module Logger haute performance

## Epic 3: Sensor Fusion & Drivers
### Story 3.1: Drivers Capteurs (Imu, Baro, Gnss, Magneto, Airspeed)
### Story 3.2: Intégration de la bibliothèque Eigen3
### Story 3.3: Intégration bibliothèque KalmanFilter
### Story 3.4: Module SensorFusion (EKF Wrapper)

## Epic 4: Communication & MAVLink
### Story 4.1: Intégration de la bibliothèque MAVLink
### Story 4.2: Développement du module RF Communication (MAVLink Telemetry)

## Epic 5: Flight & Propulsion Control
### Story 5.1: Implémentation des contrôleurs PID déterministes
### Story 5.2: FlightControlTask (Boucle 400Hz)
### Story 5.3: Implémentation du Propulsion Controller (Actuator Commands)
### Story 5.4: NavigationTask (Waypoints, Geofencing)

## Epic 6: Sécurité & Supervision
### Story 6.1: SafetyTask (FaultBus, Watchdog, Failsafe)
### Story 6.2: Mise en place CI/CD et vérification mémoire

## Epic 7: Simulation & Validation (SIL/HITL)
### Story 7.1: Infrastructure SIL (Simulation AirSim/Gazebo)
### Story 7.2: Infrastructure HITL (Hardware-in-the-Loop)
### Story 7.3: Automatisation CI/CD (Tests de Simulation)

## Epic 8: Modèles de Véhicules & Dynamique de Vol
### Story 8.1: Abstraction du modèle de véhicule (VehicleModel Interface)
### Story 8.2: Implémentation du mixage Quadcopter
### Story 8.3: Implémentation du mixage Fixed-Wing
### Story 8.4: Gestion des contraintes de vol et limites dynamiques

## Epic 9: Data Management & Persistence
### Story 9.1: Abstraction FileSystem (LittleFS/FatFS Wrapper)
### Story 9.2: Moteur de Logging haute performance
### Story 9.3: Gestionnaire de Configuration robuste
### Story 9.4: Intégrité des données (Journalisation/Checksums)

## Epic 10: Power Management & System Health
### Story 10.1: Driver Moniteur de puissance (ADC/I2C/SMBus)
### Story 10.2: Sous-module Gestion de la Batterie (BMS)
### Story 10.3: Sous-module Gestion du Fuel
### Story 10.4: Politique de sécurité énergétique (Failsafe)
### Story 10.5: Contrôle de l'alimentation des périphériques
