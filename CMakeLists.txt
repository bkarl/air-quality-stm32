cmake_minimum_required(VERSION 3.13)
set(CMAKE_TOOLCHAIN_FILE /opt/stm32-cmake/cmake/stm32_gcc.cmake)
set(STM32_CUBE_F4_PATH /opt/cubemx/STM32CubeF4-master)

project(CO2 C ASM)

find_package(CMSIS COMPONENTS STM32F401RE REQUIRED)
find_package(HAL COMPONENTS STM32F4 REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)


set(PROJECT_SOURCES
    main.c
    s8.c
    s8_hw.c
    stm32f4xx_it.c
    display.c
    display_hw.c
)

add_subdirectory("u8g2" "u8g2_build")

add_executable(CO2.elf ${PROJECT_SOURCES})
target_link_libraries(CO2.elf
    HAL::STM32::F4::RCC
    HAL::STM32::F4::GPIO
    HAL::STM32::F4::I2C
    HAL::STM32::F4::EXTI
    HAL::STM32::F4::UART
    HAL::STM32::F4::CORTEX
    CMSIS::STM32::F401RE 
    STM32::NoSys
    U8GLIB)
