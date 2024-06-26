cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME "pico_playground")
set(BUILD_NUMBER "1")

# Define an option to build for the host platform
option(BUILD_FOR_HOST "Build for the host platform" OFF)

### --------------------- HOST BUILD -------------------------------------------------
if (BUILD_FOR_HOST)
    message(STATUS "Configuring for host platform")

    # Specify the C++ compiler
    set(CMAKE_CXX_COMPILER "/usr/bin/g++")
    set(CMAKE_C_COMPILER "/usr/bin/gcc")
    add_compile_definitions(BUILD_FOR_HOST)
    # Set compiler flags for host build
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

    enable_testing()
    # Common source code directory
    set(COMMON_CODE_DIRECTORY "${CMAKE_SOURCE_DIR}/src/common")

    # Name the project
    project(${PROJECT_NAME}
        LANGUAGES C CXX ASM
        DESCRIPTION "Embedded Software Pico Playground"
    )

    # Path to Google Test
    set(GTEST_ROOT "${CMAKE_SOURCE_DIR}/external/googletest")
    set(TESTS_ROOT "${CMAKE_SOURCE_DIR}/tests")
    set(GPS_LIBRARY "${CMAKE_SOURCE_DIR}/lib/adafruit_gps")
    set(MOCKS_PATH "${CMAKE_SOURCE_DIR}/tests/mocks")
    add_subdirectory(${GTEST_ROOT})
    add_subdirectory(${TESTS_ROOT})
    add_subdirectory(${GPS_LIBRARY})


    message(STATUS "Configuring for host platform complete !!")
else()### --------------------- TARGET BUILD -------------------------------------------------
    message(STATUS "Configuring for target platform")

    # Set the Pico board to use
    set(PICO_BOARD pico_w)

    # Set application name(s) and version(s)
    set(FREERTOS_TIMER_APP "FreeRtosTimerApp")
    set(FREERTOS_TIMER_APP_VERSION_NUMBER "1.0")

    # Specify the app(s) source code directories
    set(FREERTOS_TIMER_APP_SRC_DIRECTORY "${CMAKE_SOURCE_DIR}/src/freertos_timer_app")
    set(PIMORONI_ROOT "${CMAKE_SOURCE_DIR}/external/pimoroni-pico")
    set(MICROROS_ROOT "${CMAKE_SOURCE_DIR}/external/micro_ros_raspberrypi_pico_sdk")
    set(PCA9685_CORE_TEST "${CMAKE_SOURCE_DIR}/lib/pca9685_motor_control")
    set(GPS_LIBRARY "${CMAKE_SOURCE_DIR}/lib/adafruit_gps")

    # Common source code directory
    set(COMMON_CODE_DIRECTORY "${CMAKE_SOURCE_DIR}/src/common")

    # Set debugging on debug builds
    add_compile_definitions($<$<CONFIG:Debug>:DEBUG=1>)

    # Set env variable 'PICO_SDK_PATH' to the local Pico SDK
    set(ENV{PICO_SDK_PATH} "${CMAKE_SOURCE_DIR}/external/pico-sdk")

    # Set FreeRTOS directories
    set(FREERTOS_CFG_DIRECTORY "${CMAKE_SOURCE_DIR}/src/config")
    set(FREERTOS_SRC_DIRECTORY "${CMAKE_SOURCE_DIR}/external/FreeRTOS-Kernel")

    # Include the Pico SDK
    include(pico_sdk_import.cmake)

    # Name the project
    project(${PROJECT_NAME}
        LANGUAGES C CXX ASM
        DESCRIPTION "Embedded Software Pico Playground"
    )

    # Initialize the Pico SDK
    pico_sdk_init()

    # Add FreeRTOS as a library
    add_library(FreeRTOS STATIC
        ${FREERTOS_SRC_DIRECTORY}/event_groups.c
        ${FREERTOS_SRC_DIRECTORY}/list.c
        ${FREERTOS_SRC_DIRECTORY}/queue.c
        ${FREERTOS_SRC_DIRECTORY}/stream_buffer.c
        ${FREERTOS_SRC_DIRECTORY}/tasks.c
        ${FREERTOS_SRC_DIRECTORY}/timers.c
        ${FREERTOS_SRC_DIRECTORY}/portable/MemMang/heap_3.c
        ${FREERTOS_SRC_DIRECTORY}/portable/GCC/ARM_CM0/port.c
    )

    # Build FreeRTOS
    target_include_directories(FreeRTOS PUBLIC
        ${FREERTOS_CFG_DIRECTORY}/
        ${FREERTOS_SRC_DIRECTORY}/include
        ${FREERTOS_SRC_DIRECTORY}/portable/GCC/ARM_CM0
    )

    # Include the apps' source code
    add_subdirectory(${FREERTOS_TIMER_APP_SRC_DIRECTORY})
    add_subdirectory(${PIMORONI_ROOT})
    add_subdirectory(${MICROROS_ROOT})
    add_subdirectory(${PCA9685_CORE_TEST})
    add_subdirectory(${GPS_LIBRARY})

    message(STATUS "Configuring for target platform complete !!")
endif()

message(STATUS "CMake configuration complete!")
