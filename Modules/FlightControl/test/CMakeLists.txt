if(CMAKE_CROSSCOMPILING)
    # Build cross-compiled test
    add_executable(PidController_test PidController_test.cpp)
    target_link_libraries(PidController_test PRIVATE FlightControl WrapperTypes)
else()
    # Build native host test
    project(FlightControlTests LANGUAGES CXX)
    add_executable(PidController_test PidController_test.cpp)
    target_compile_features(PidController_test PRIVATE cxx_std_23)
    target_include_directories(PidController_test PRIVATE ..)
endif()
