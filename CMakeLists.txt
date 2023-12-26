cmake_minimum_required(VERSION 3.27)

include(pico_sdk_import.cmake)

project(LCDLibrary C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_library(LCDLibrary STATIC library.cpp)

target_link_libraries(LCDLibrary pico_stdlib hardware_spi hardware_pwm)
