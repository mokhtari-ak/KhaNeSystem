add_executable(sil_sim sil_main.cpp integration_tests.cpp)
target_link_libraries(sil_sim PRIVATE Wrappers InternalEventBus GTest::gtest_main)
