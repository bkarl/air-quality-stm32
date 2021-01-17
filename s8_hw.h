#pragma once

#include "stm32f4xx_hal.h"

#define S8_USART_INSTANCE                   USART2
#define S8_ENABLE_UART_CLOCK                __HAL_RCC_USART2_CLK_ENABLE

#define S8_USART_PORT                       GPIOA
#define S8_USART_GPIO_CLOCK                 __HAL_RCC_GPIOA_CLK_ENABLE
#define S8_USART_TX                         GPIO_PIN_2
#define S8_USART_RX                         GPIO_PIN_3
#define S8_GPIO_UART_AF                     GPIO_AF7_USART2

#define S8_CALIB_BUTTON_PIN                 GPIO_PIN_0     
#define S8_CALIB_BUTTON_PORT                GPIOA
#define S8_ENABLE_CALIB_BUTTON_CLOCK        __HAL_RCC_GPIOA_CLK_ENABLE

#define S8_CALIB_PORT                       GPIOC
#define S8_CALIB_PIN                        GPIO_PIN_10
#define S8_ENABLE_CALIB_PORT_CLOCK          __HAL_RCC_GPIOC_CLK_ENABLE

#define DISPLAY_I2C_PORT                    GPIOB
#define DISPLAY_SCL_PIN                     GPIO_PIN_8
#define DISPLAY_SDA_PIN                     GPIO_PIN_9

#define ANSWER_LENGTH_FOR_CO2_AND_STATUS    13
#define MAX_VALID_CO2_VALUE                 10000

#define CALIBRATION_BUTTON_PRESSED          1
#define CALIBRATION_BUTTON_NOT_PRESSED      0

void s8_hw_init();
void s8_start_background_calibration();
int s8_get_status_and_sensor_reading(uint8_t *result);
int s8_is_calibration_button_pressed();
