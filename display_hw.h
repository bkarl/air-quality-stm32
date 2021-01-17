#pragma once

#include "u8g2.h"
#include "stm32f4xx_hal.h"

#define DISPLAY_RES_PORT    GPIOC
#define DISPLAY_RES_PIN     GPIO_PIN_6

#define DISPLAY_DC_PORT     GPIOC
#define DISPLAY_DC_PIN      GPIO_PIN_7

#define DISPLAY_I2C_PORT    GPIOB
#define DISPLAY_SCL_PIN     GPIO_PIN_8
#define DISPLAY_SDA_PIN     GPIO_PIN_9

void display_hw_init();
uint8_t display_hw_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t display_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);