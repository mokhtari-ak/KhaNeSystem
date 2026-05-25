cmake_minimum_required(VERSION 3.22)

# Project Settings
project(KhaNeSystem C CXX ASM)

# Toolchain check
if(NOT CMAKE_TOOLCHAIN_FILE)
    message(FATAL_ERROR "No toolchain file provided. Use -DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake")
endif()

# Compiler Flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Embedded flags for Cortex-M4F
# original set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
set(CPU_FLAGS "-mcpu=cortex-m4")
add_compile_options(${CPU_FLAGS} -fno-exceptions -fno-rtti -fno-unwind-tables -ffunction-sections -fdata-sections)
add_link_options(${CPU_FLAGS} --specs=nosys.specs -Wl,--gc-sections)

# Release optimizations
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(EIGEN_NO_DEBUG EIGEN_NO_MALLOC)
endif()

# Dependencies
add_subdirectory(Modules/eigen)
add_subdirectory(Modules/KalmanFilterLibrary)

# Modules Applicatifs
add_subdirectory(Modules/InternalEventBus)
add_subdirectory(Modules/SensorFusion)
add_subdirectory(Modules/FlightControl)
add_subdirectory(Modules/Navigation)
add_subdirectory(Modules/Safety)
add_subdirectory(Modules/Calibration)
add_subdirectory(Modules/ConfigurationManager)

# Build artifacts
add_executable(${CMAKE_PROJECT_NAME} "")

# Modules systÃ¨me
add_subdirectory(cmake/stm32cubemx)
add_subdirectory(Libs/HardwareAccessLayer/STM32F4HAL)
add_subdirectory(Libs/wrappers/WrapperPolicies)
add_subdirectory(Libs/wrappers/WrapperTypes)
add_subdirectory(Libs/wrappers)

# Include directories
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    Core/Inc
    Libs/HardwareAccessLayer
    Libs/HardwareAccessLayer/STM32F4HAL
    Libs/wrappers/WrapperPolicies
    Libs/wrappers/WrapperTypes
    Libs/wrappers
    Modules/eigen
    Modules/KalmanFilterLibrary
    Modules/InternalEventBus
    Modules/SensorFusion
    Modules/FlightControl
    Modules/Navigation
    Modules/Safety
    Modules/Calibration
    Modules/ConfigurationManager
)

# Sources
#target_sources(${CMAKE_PROJECT_NAME} PRIVATE
#    Core/Src/main.cpp
#    Core/Src/stm32f4xx_it.c
#    Core/Src/stm32f4xx_hal_msp.c
#    Core/Src/system_stm32f4xx.c
#    /startup_stm32f4xx.s
#)

# Linking
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    STM32F4HAL
    WrapperPolicies
    WrapperTypes
    Wrappers
    Eigen3::Eigen
)

# Memory Check Script
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/_bmad/scripts/check_memory.py ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map
    COMMENT "Verifying memory layout..."
)



