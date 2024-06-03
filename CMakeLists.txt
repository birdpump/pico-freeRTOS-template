cmake_minimum_required(VERSION 3.12)

set(PICO_TOOLCHAIN_PATH "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/gcc-arm-none-eabi/")

set(PICO_SDK_PATH "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/pico-sdk/")
# Name project
SET(ProjectName f1_pico-rtos)



# Set any variables required for importing libraries
SET(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/FreeRTOS-Kernel)

# Import those libraries
include(pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Define project
project(${ProjectName} CXX C ASM)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(lib/pico_neopixels)

# Add subdirectories
add_subdirectory(src)
