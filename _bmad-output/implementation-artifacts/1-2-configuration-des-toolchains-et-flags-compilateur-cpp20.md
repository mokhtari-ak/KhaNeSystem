# Story 1.2: Configuration des toolchains et flags compilateur C++20

## Status
- **ID:** 1.2
- **Key:** 1-2-configuration-des-toolchains-et-flags-compilateur-cpp20
- **Status:** review
- **Epic:** Epic 1: Socle Matériel & BSP
- **Last Updated:** 2026-05-31

## Requirements
### User Story
En tant que développeur embarqué, je souhaite configurer précisément la toolchain ARM GCC et les flags de compilation afin de garantir la conformité aux standards du projet (C++20, sécurité, performance FPU) et d'assurer un binaire optimisé pour le STM32F407VG.

### Acceptance Criteria (BDD)
- **GIVEN** un projet STM32F4 généré avec CMake
- **WHEN** je configure les flags de compilation dans le fichier toolchain ou le `CMakeLists.txt` racine
- **THEN** le compilateur doit utiliser le standard C++20 (`-std=c++20`)
- **AND** les exceptions (`-fno-exceptions`) et le RTTI (`-fno-rtti`) doivent être désactivés
- **AND** le support FPU matériel doit être activé (`-mfloat-abi=hard -mfpu=fpv4-sp-d16`)
- **AND** les optimisations de taille et de performance doivent être équilibrées (`-Os`)
- **AND** toutes les erreurs de type `double` doivent être signalées (pas d'émulation logicielle)

## Tasks/Subtasks
- [x] Vérifier la configuration `gcc-arm-none-eabi.cmake`
- [x] Appliquer les flags C++20, désactiver exceptions/RTTI
- [x] Configurer les flags FPU et architecture (cortex-m4, mthumb)
- [x] Nettoyer `CMakeLists.txt` racine des redondances de flags
- [x] Compiler et vérifier la cohérence

## Dev Agent Record
- **Completion Notes:**
    - Toolchain GCC ARM configurée pour Cortex-M4F.
    - Flags `-std=c++20`, `-fno-exceptions`, `-fno-rtti` appliqués globalement via la toolchain.
    - Support FPU hardware (`-mfloat-abi=hard`) activé.
    - Flags stricts (`-Wall`, `-Wextra`, `-Wdouble-promotion`) activés.
- **Change Log:**
    - Modifié `cmake/gcc-arm-none-eabi.cmake` pour centraliser les flags de build.
    - Nettoyé `CMakeLists.txt` des flags redondants.
