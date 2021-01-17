#include "s8_hw.h"

UART_HandleTypeDef USART2Handle = {0};
const uint8_t S8_getStatusAndCO2ReadingCmd[] = {0xFE, 0x04, 0x00, 0x00, 0x00, 0x04, 0xE5, 0xC6};

void s8_hw_init() {

    //enable clocks
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    //RX/TX Pins
    S8_USART_GPIO_CLOCK();
    GPIO_InitStructure.Pin       = S8_USART_TX | S8_USART_RX;
    GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull      = GPIO_NOPULL;
    GPIO_InitStructure.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStructure.Alternate = S8_GPIO_UART_AF;
    
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    //Taster
    S8_ENABLE_CALIB_BUTTON_CLOCK();
    GPIO_InitStructure.Pin       = S8_CALIB_BUTTON_PIN;
    GPIO_InitStructure.Mode      = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull      = GPIO_PULLUP;
    GPIO_InitStructure.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStructure.Alternate = 0;
    HAL_GPIO_Init(S8_CALIB_BUTTON_PORT, &GPIO_InitStructure);

    //bCAL
    S8_ENABLE_CALIB_PORT_CLOCK();
    GPIO_InitStructure.Pin       = S8_CALIB_PIN;
    GPIO_InitStructure.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull      = GPIO_NOPULL;
    GPIO_InitStructure.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStructure.Alternate = 0;
  
    HAL_GPIO_Init(S8_CALIB_PORT, &GPIO_InitStructure);
    HAL_GPIO_WritePin(S8_CALIB_PORT, S8_CALIB_PIN, GPIO_PIN_SET); //Calibration is active low
    
    //USART
    S8_ENABLE_UART_CLOCK();
    USART2Handle.Instance          = USART2;
    USART2Handle.Init.BaudRate     = 9600;
    USART2Handle.Init.WordLength   = UART_WORDLENGTH_8B;
    USART2Handle.Init.StopBits     = UART_STOPBITS_1;
    USART2Handle.Init.Parity       = UART_PARITY_NONE;
    USART2Handle.Init.Mode         = UART_MODE_TX_RX;
    USART2Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    USART2Handle.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&USART2Handle);
}

int s8_get_status_and_sensor_reading(uint8_t *result)
{
    int status = 0;
    status = HAL_UART_Transmit(&USART2Handle, (uint8_t*)S8_getStatusAndCO2ReadingCmd, sizeof(S8_getStatusAndCO2ReadingCmd), 1000);
    status |= HAL_UART_Receive(&USART2Handle,result,ANSWER_LENGTH_FOR_CO2_AND_STATUS,1000);
    
    return status;
}

int s8_is_calibration_button_pressed()
{
    if (HAL_GPIO_ReadPin(S8_CALIB_BUTTON_PORT, S8_CALIB_BUTTON_PIN) == GPIO_PIN_RESET) 
        return CALIBRATION_BUTTON_PRESSED;
    return CALIBRATION_BUTTON_NOT_PRESSED;
}

void s8_start_background_calibration()
{
    if (HAL_GPIO_ReadPin(S8_CALIB_BUTTON_PORT, S8_CALIB_BUTTON_PIN) == GPIO_PIN_SET)
        return;

    HAL_GPIO_WritePin(S8_CALIB_PORT, S8_CALIB_PIN, GPIO_PIN_RESET);
    HAL_Delay(5000);
    HAL_GPIO_WritePin(S8_CALIB_PORT, S8_CALIB_PIN, GPIO_PIN_SET);
}