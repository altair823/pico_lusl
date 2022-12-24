# # Generated Cmake Pico project file

# cmake_minimum_required(VERSION 3.13)

# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 17)
# set(PICO_BOARD pico_w)

# # Initialise pico_sdk from installed location
# # (note this can come from environment, CMake cache etc)
# set(PICO_SDK_PATH "/Users/altair823/pico-sdk")

# # Pull in Raspberry Pi Pico SDK (must be before project)
# include(pico_sdk_import.cmake)

# project(pico-lusl C CXX ASM)

# set(PICO_CXX_ENABLE_RTTI 1)

# # Initialise the Raspberry Pi Pico SDK
# pico_sdk_init()

# add_subdirectory(lib/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)

# include_directories(
#     ${CMAKE_CURRENT_SOURCE_DIR}/includes
#     ${CMAKE_CURRENT_SOURCE_DIR}/src
#     )

# # Add executable. Default name is the project name, version 0.1

# add_executable(
#     pico-lusl 
#     pico-lusl.c
#     includes/binary.h
#     includes/variables.h
#     includes/version.h
#     )

# pico_set_program_name(pico-lusl "pico-lusl")
# pico_set_program_version(pico-lusl "0.1")
# pico_add_extra_outputs(${PROJECT_NAME})

# pico_enable_stdio_uart(pico-lusl 1)
# pico_enable_stdio_usb(pico-lusl 1)

# # Add the standard library to the build
# target_link_libraries(pico-lusl 
# pico_stdlib 
# pico_cyw43_arch_none 
# FatFs_SPI
# )

# pico_add_extra_outputs(pico-lusl)

# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C%2B%2B standards
project(sd_fat_spi C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
    pico-lusl.c
    src/hw_config.c
)

# Tell CMake where to find other source code
add_subdirectory(no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    FatFs_SPI
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)