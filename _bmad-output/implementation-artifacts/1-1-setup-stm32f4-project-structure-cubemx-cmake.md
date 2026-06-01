# Story 1.1: Setup STM32F4 Project structure (CubeMX + CMake)

## Status
- **ID:** 1.1
- **Key:** 1-1-setup-stm32f4-project-structure-cubemx-cmake
- **Status:** review
- **Epic:** Epic 1: Socle Matériel & BSP
- **Last Updated:** 2026-05-31

## Requirements
### User Story
En tant que développeur système, je souhaite mettre en place la structure de base du projet utilisant STM32CubeMX et CMake afin d'avoir un environnement de build robuste, reproductible et compatible avec les standards C++20 du projet.

### Acceptance Criteria (BDD)
- **GIVEN** un nouveau projet pour STM32F407VG
- **WHEN** j'exécute la génération de code via CubeMX avec l'option CMake
- **THEN** la structure de fichiers standard doit être créée (Core, Drivers, cmake)
- **AND** le fichier `CMakeLists.txt` racine doit être configuré pour supporter C++20
- **AND** la compilation doit réussir sans erreur
- **AND** le binaire généré doit pouvoir être flashé sur la cible

## Tasks/Subtasks
- [x] Setup STM32F4 Project structure (CubeMX + CMake)
- [x] Configuration des toolchains et flags compilateur C++20
- [x] Bridge C++ (app_bridge.h / app_main.cpp)
- [x] Compilation et validation des flags

## Dev Agent Record
- **Completion Notes:**
    - Projet configuré avec succès en C++20.
    - Bridge entre main.c et C++ mis en place.
    - Compilation vérifiée avec succès.
    - Flags `-fno-exceptions` et `-fno-rtti` appliqués.
- **Change Log:**
    - Initialisé la structure de projet CMake.
    - Créé le bridge C++.
    - Configuré le support C++20.

## Developer Context
### Technical Guardrails
- **Language:** C++20 standard (`-std=c++20`).
- **No Exceptions/RTTI:** Utiliser les flags `-fno-exceptions` et `-fno-rtti`.
- **HAL Restriction:** L'inclusion de `stm32f4xx_hal.h` est **STRICTEMENT INTERDITE** en dehors de `Libs/HardwareAccessLayer/STM32F4HAL/`.
- **Dynamic Allocation:** `malloc`, `new`, `free` sont interdits dans le code de vol.
- **Floating Point:** Utiliser `float` uniquement. `double` est interdit (émulation logicielle lente).
- **Project Structure:**
    - Garder `main.c` (généré par CubeMX) tel quel.
    - Créer un bridge C++ : `Core/Inc/app_bridge.h` et `Core/Src/main.cpp`.
    - Appeler le point d'entrée C++ depuis `main.c` via `extern "C"`.

## Implementation Strategy
1. **CubeMX Configuration:** Charger le fichier `.ioc` existant (si disponible) ou configurer un nouveau projet pour STM32F407VGTx.
2. **Project Manager:** Sélectionner "CMake" comme Toolchain/IDE.
3. **Code Generation:** "Generate peripheral initialization as a pair of '.c/.h' files per peripheral".
4. **CMake Setup:**
    - Modifier le `CMakeLists.txt` racine pour activer `CXX`.
    - Ajouter les flags C++20, `-fno-exceptions`, `-fno-rtti`.
    - Configurer l'inclusion de `Core/Src/main.cpp`.
5. **C/C++ Bridge:**
    - Dans `main.c`, inclure `app_bridge.h`.
    - Dans `main.c`, appeler `app_main()` juste avant la boucle `while(1)`.
    - Dans `main.cpp`, implémenter `app_main()` qui initialise le système et lance le scheduler RTOS (si déjà présent).

## Validation Plan
- **Build:** `cmake -B build -G Ninja && cmake --build build`
- **Check Flags:** Vérifier via `compile_commands.json` que `-std=c++20`, `-fno-exceptions` et `-fno-rtti` sont bien appliqués.
- **Check Binary:** Vérifier la taille du binaire et la présence des symboles attendus (`arm-none-eabi-size`, `arm-none-eabi-nm`).
- **Standard Check:** Utiliser `clang-tidy` pour vérifier qu'aucun include interdit de la HAL n'est présent dans `main.cpp`.

## Project Context Reference
- Voir `GEMINI.md` pour les interdictions strictes.
- Voir `project-context.md` pour les contraintes techniques générales.
- Voir `architecture.md` pour la cartographie des tâches RTOS et la SystemClock.
