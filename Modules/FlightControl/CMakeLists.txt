add_library(FlightControl INTERFACE)
target_include_directories(FlightControl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(FlightControl INTERFACE STM32F4HAL)

add_subdirectory(test)



