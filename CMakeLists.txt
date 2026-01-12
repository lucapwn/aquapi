# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)

set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

set(ProjectName main)
set(PICO_BOARD pico_w CACHE STRING "Board type")
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include/libraries/FreeRTOS-Kernel)

include(pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(${ProjectName} C CXX ASM)

pico_sdk_init()

file(GLOB APP_FILES "src/app/*.c")
file(GLOB DRIVER_FILES "src/drivers/*.c")
file(GLOB NETWORK_FILES "src/network/*.c")
file(GLOB SERVICE_FILES "src/services/*.c")
file(GLOB SSD1306_FILE "include/libraries/pico-ssd1306/ssd1306.c")
file(GLOB SDCARD_FILES "include/libraries/pico-sdcard/src/*.c")

add_compile_options(
    -Wall
    -Wno-format
    -Wno-unused-result
    -Wno-unused-function
    -Wno-maybe-uninitialized
)

add_executable(${ProjectName}
    ${APP_FILES}
    ${DRIVER_FILES}
    ${NETWORK_FILES}
    ${SERVICE_FILES}
    ${SSD1306_FILE}
    ${SDCARD_FILES}
)

pico_set_program_name(${ProjectName} "main")
pico_set_program_version(${ProjectName} "0.1")

pico_enable_stdio_uart(${ProjectName} 1)
pico_enable_stdio_usb(${ProjectName} 1)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/include/app
    ${CMAKE_CURRENT_LIST_DIR}/include/drivers
    ${CMAKE_CURRENT_LIST_DIR}/include/network
    ${CMAKE_CURRENT_LIST_DIR}/include/services
)

target_link_libraries(${ProjectName}
    pico_cyw43_arch_lwip_poll
    pico_lwip_mbedtls
    pico_lwip_mqtt
    pico_multicore
    pico_mbedtls
    pico_stdlib
    hardware_gpio
    hardware_adc
    hardware_spi
    hardware_i2c
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)

pico_add_extra_outputs(${ProjectName})
