cmake_minimum_required(VERSION 3.14)

# Make project data accessible to compiler
add_compile_definitions(APP_NAME="${FREERTOS_TIMER_APP}")
add_compile_definitions(APP_VERSION="${FREERTOS_TIMER_APP_VERSION_NUMBER}")
add_compile_definitions(BUILD_NUM=${BUILD_NUMBER})

# Include app source code file(s)
add_executable(${FREERTOS_TIMER_APP}
    ${FREERTOS_TIMER_APP_SRC_DIRECTORY}/main.cpp
    ${COMMON_CODE_DIRECTORY}/utils.cpp
)

# Link to built libraries
target_link_libraries(${FREERTOS_TIMER_APP} LINK_PUBLIC
    pico_stdlib
    pico_cyw43_arch_none
    FreeRTOS)

target_include_directories(${FREERTOS_TIMER_APP} PUBLIC
    ${COMMON_CODE_DIRECTORY}
)

# Enable/disable STDIO via USB and UART
pico_enable_stdio_usb(${FREERTOS_TIMER_APP} 1)
pico_enable_stdio_uart(${FREERTOS_TIMER_APP} 1)

# Enable extra build products
pico_add_extra_outputs(${FREERTOS_TIMER_APP})
