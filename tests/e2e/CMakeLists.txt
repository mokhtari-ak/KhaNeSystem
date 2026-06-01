add_executable(sil_sim sil_main.cpp)
target_link_libraries(sil_sim PRIVATE Wrappers InternalEventBus)
