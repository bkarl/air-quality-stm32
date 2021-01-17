#include "display_hw.h"

void display_init_gpio();
void display_init_i2c();
I2C_HandleTypeDef I2cHandle;
extern u8g2_t u8g2;

void display_hw_init() 
{
    display_init_gpio();
    display_init_i2c();
}

void display_init_gpio() 
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.Pin       = DISPLAY_SDA_PIN | DISPLAY_SCL_PIN;
    GPIO_InitStructure.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStructure.Pull      = GPIO_NOPULL;
    GPIO_InitStructure.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStructure.Alternate = GPIO_AF4_I2C1;
    
    HAL_GPIO_Init(DISPLAY_I2C_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin       = DISPLAY_RES_PIN;
    GPIO_InitStructure.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull      = GPIO_NOPULL;
    GPIO_InitStructure.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStructure.Alternate = 0;

    HAL_GPIO_Init(DISPLAY_RES_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin       = DISPLAY_DC_PIN;
    GPIO_InitStructure.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull      = GPIO_NOPULL;
    GPIO_InitStructure.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStructure.Alternate = 0;

    HAL_GPIO_Init(DISPLAY_DC_PORT, &GPIO_InitStructure);
    HAL_GPIO_WritePin(DISPLAY_DC_PORT, DISPLAY_DC_PIN, RESET);
}

void display_init_i2c()
{
    __HAL_RCC_I2C1_CLK_ENABLE();

    I2cHandle.Instance             = I2C1;
    I2cHandle.Init.ClockSpeed      = 100000;
    I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_2;
    I2cHandle.Init.OwnAddress1     = 0;
    I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    I2cHandle.Init.OwnAddress2     = 0xFF;
    I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&I2cHandle);
}

uint8_t display_hw_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch (msg)
  {
  case U8X8_MSG_GPIO_AND_DELAY_INIT:
    //HAL_Delay(1);
    break;
  case U8X8_MSG_DELAY_MILLI:
    HAL_Delay(arg_int);
    break;
  case U8X8_MSG_GPIO_DC:
    HAL_GPIO_WritePin(DISPLAY_DC_PORT, DISPLAY_DC_PIN, arg_int);
    break;
  case U8X8_MSG_GPIO_RESET:
    HAL_GPIO_WritePin(DISPLAY_RES_PORT, DISPLAY_RES_PIN, arg_int);
    break;
  }
  return 1;
}

uint8_t display_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) 
{
  static uint8_t buffer[32];		/* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
  static uint8_t buf_idx;
  uint8_t *data;
  int status;
  uint8_t address = u8g2_GetI2CAddress(&u8g2);

  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;      
      while( arg_int > 0 )
      {
        buffer[buf_idx++] = *data;
        data++;
        arg_int--;
      }      
      break;
    case U8X8_MSG_BYTE_INIT:
      /* add your custom code to init i2c subsystem */
      break;
    case U8X8_MSG_BYTE_SET_DC:
      /* ignored for i2c */
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      buf_idx = 0;
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        //the 7 bit address must be shifted to the left before calling the interface
        status = HAL_I2C_Master_Transmit(&I2cHandle, address, (uint8_t*)buffer, buf_idx, 10000);
        if (status != HAL_OK)
          while(1);
      break;
    default:
      return 0;
  }
  return 1;
}