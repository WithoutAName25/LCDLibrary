cmake_minimum_required(VERSION 3.27)

include(pico_sdk_import.cmake)

project(LCDLibrary C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_library(LCDLibrary STATIC
        utils.cpp utils.h
        LCDController.cpp LCDController.h
        LCDGraphics.cpp LCDGraphics.h
        LCDDirectGraphics.cpp LCDDirectGraphics.h
        include/LCDLibrary.h
        SPIInterface.cpp
        SPIInterface.h
)

pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/spi.pio)

target_include_directories(LCDLibrary PUBLIC
    include
)

target_link_libraries(LCDLibrary pico_stdlib hardware_spi hardware_pio hardware_pwm)
