cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME "pico_playground_tests")
set(BUILD_NUMBER "1")

# Enable testing
enable_testing()

# Add a simple test executable
add_executable(${PROJECT_NAME} 
    test_main.cpp
    utils_tests.cpp
    nmea_parse_tests.cpp
    ${COMMON_CODE_DIRECTORY}/utils.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${COMMON_CODE_DIRECTORY}
)

# Link test libraries
target_link_libraries(${PROJECT_NAME}
    gtest
    gtest_main
    Adafruit_Gps_Library
)

# Add tests
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})


message(STATUS "Configuring for host platform complete !!")
