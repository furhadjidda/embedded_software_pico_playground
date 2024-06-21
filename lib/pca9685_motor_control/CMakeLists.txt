cmake_minimum_required(VERSION 3.14)
# Set app name(s) and version(s)
# Set project name and version
project(pca9685_core_test VERSION 0.1)


set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Directory names and path
set(PCA9685 "${CMAKE_CURRENT_SOURCE_DIR}")

# Define compile-time constants
add_compile_definitions(APP_NAME="${PROJECT_NAME}")
add_compile_definitions(APP_VERSION="${PROJECT_VERSION}")
add_compile_definitions(BUILD_NUM=${BUILD_NUMBER})

###################################################################
# Building a library
###################################################################

add_library(pca9685 STATIC
    pca9685.cpp
)

# Include directories for the library
target_include_directories(pca9685 PUBLIC
    ${PCA9685}
)

# Link the test application with the PCA9685 library and other dependencies
target_link_libraries(pca9685 PUBLIC
    pico_stdlib
    pico_cyw43_arch_none
    hardware_i2c
)

###################################################################
###################################################################

# Building the test application
###################################################################
add_executable(pca9685_test
    pca9685_test.cpp
)

# Link the test application with the PCA9685 library and other dependencies
target_link_libraries(pca9685_test PUBLIC
    pca9685
)

# Include directories for the test application
target_include_directories(pca9685_test PUBLIC
    ${PCA9685}
)

# Define compile-time constants for the test application
target_compile_definitions(pca9685_test PUBLIC
    APP_NAME="${PROJECT_NAME}"
    APP_VERSION="${PROJECT_VERSION}"
    BUILD_NUM=${BUILD_NUMBER}
)
###################################################################


# Additional compiler flags
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")

# Enable/disable STDIO via USB and UART for the test application
pico_enable_stdio_usb(pca9685_test 1)
pico_enable_stdio_uart(pca9685_test 1)

# Enable extra build products for the test application
pico_add_extra_outputs(pca9685_test)
