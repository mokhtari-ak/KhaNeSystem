---
stepsCompleted: [1]
inputDocuments: ["_bmad-output/planning-artifacts/prds/prd-KhaNeSystem-2026-05-25/prd.md", "project-context.md"]
workflowType: 'architecture'
project_name: 'KhaNeSystem'
user_name: 'KIV'
date: '2026-05-25'
---

# Architecture Decision Document

_This document builds collaboratively through step-by-step discovery. Sections are appended as we work through each architectural decision together._


## 1. RTOS Task Mapping Analysis

| Task | Freq | Prio | Stack | Deadline | Purpose |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **SensorTask** | 1000 Hz | 7/7 | 2 KB | 1 ms | Lecture IMU/Baro/Pitot (DMA) |
| **SafetyTask** | 400 Hz | 7/7 | 2 KB | 2.5 ms | FaultBus, ResponseMatrix, IWDG |
| **EkfTask** | 250 Hz | 6/7 | 4 KB | 4 ms | Fusion, StateVector, EKF (float) |
| **FlightControlTask**| 400 Hz | 6/7 | 3 KB | 2.5 ms | PID, ActuatorCmd |
| **RcInputTask** | 200 Hz | 6/7 | 1 KB | 5 ms | Décodage SBUS/CRSF |
| **NavigationTask** | 50 Hz | 5/7 | 3 KB | 20 ms | Guidage Waypoints |
| **LoggingTask** | 100 Hz | 2/7 | 4 KB | 10 ms | SD Card (DMA, non-bloquant) |
| **TelemetryTask** | 10 Hz | 3/7 | 2 KB | 100 ms | MAVLink Downlink |

### Analysis Highlights:
- **Prioritization**: High-priority tasks (Safety, Sensor, Flight) are correctly identified to meet tight deadlines (1ms - 2.5ms).
- **Stack Usage**: Total requested stack ~23 KB. STM32F407VG has 192 KB total RAM. This is well within safe margins, allowing room for heap/DMA buffers.
- **CPU Budget**: Estimations indicate ~62% load. A 20% margin remains for ISRs and overhead.
- **Constraints**: Deadline alignment validated for 168 MHz operation.

## 3. SystemClock Specification

La source de temps monotonique est la fondation du déterminisme RTOS et de la télémétrie.

### Spécifications Techniques
- **Source Matérielle**: TIM2 (32-bit), configuré à 168 MHz (via prescaler pour 1 MHz ou résolution 1 µs).
- **Interface**: `ISystemClock` (Concept C++20).
- **Unités Fortes**: Utilisation de types dédiés (`Microseconds`, `Milliseconds`, `Hertz`) pour éviter toute confusion.
- **Gestion Overflow**: TIM2 est un timer 32-bit en free-running, gérant naturellement l'overflow sur de longues périodes sans besoin de rollover complexe.

### Interface `ISystemClock`
```cpp
template<typename T>
concept ISystemClock = requires(T t, Microseconds from) {
    { t.now_us() } -> std::same_as<Microseconds>;
    { t.now_ms() } -> std::same_as<Milliseconds>;
    { t.elapsed_us(from) } -> std::same_as<Microseconds>;
};
```

### Conception
- **Injection de dépendance**: L'interface est injectée dans les modules via template pour permettre le remplacement par un `MockClock` lors des tests SIL (AirSim) ou unitaires.
- **Déterminisme**: Aucune fonction de temps n'est bloquante. L'accès au registre TIM2 se fait par lecture directe (overhead ~cycles CPU).
- **Strong Typedefs**: Empêche les erreurs de calcul (`dt` en secondes vs millisecondes) à la compilation.
