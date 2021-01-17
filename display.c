#include "display.h"
#include "display_hw.h"

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include "s8.h"

u8g2_t u8g2;

void display_init()
{
    display_hw_init();
    u8g2_Setup_ssd1306_i2c_128x64_noname_2(&u8g2, U8G2_R0, display_hw_i2c, display_hw_gpio_and_delay);  // init u8g2 structure
    u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
    u8g2_SetPowerSave(&u8g2, 0); // wake up display
    
    u8g2_SetFont(&u8g2, u8g2_font_fur17_tf);
}

void display_main_loop()
{
  char stringPPM[32];
  s8_calibrate_if_requested();
  s8_get_current_reading_as_string(stringPPM);
  
  unsigned int strWidth = u8g2_GetStrWidth(&u8g2, stringPPM);
  u8g2_FirstPage(&u8g2);
  do {
      u8g2_DrawStr(&u8g2, (128/2)-strWidth/2, 42, stringPPM);        
    } while ( u8g2_NextPage(&u8g2) );
    HAL_Delay(1000);
}
