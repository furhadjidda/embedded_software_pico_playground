cmake_minimum_required(VERSION 3.14)
# Set app name(s) and version(s)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(Motor_Control "pca9685_core_test")
set(Motor_Control_VERSION_NUMBER "0.1")


# Directory names and path
set(PCA9685 "${CMAKE_CURRENT_SOURCE_DIR}")
# Make project data accessible to compiler
add_compile_definitions(APP_NAME="${Motor_Control}")
add_compile_definitions(APP_VERSION="${Motor_Control_VERSION_NUMBER}")
add_compile_definitions(BUILD_NUM=${BUILD_NUMBER})


# Include app source code file(s)
add_executable(${Motor_Control}
    pca9685_test.cpp
    pca9685.cpp
)

# Link to built libraries
target_link_libraries(${Motor_Control} LINK_PUBLIC
    pico_stdlib
    pico_cyw43_arch_none
    hardware_i2c
)
target_include_directories(${Motor_Control} PUBLIC
    ${PCA9685}
)

SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")
# Enable/disable STDIO via USB and UART
pico_enable_stdio_usb(${Motor_Control} 1)
pico_enable_stdio_uart(${Motor_Control} 1)

# Enable extra build products
pico_add_extra_outputs(${Motor_Control})
