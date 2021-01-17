#include "s8_hw.h"
#include "s8.h"
#include <stdio.h>
#include <string.h>

const char* unknownReadingString = "? ppm";

void s8_init()
{
    s8_hw_init();
}

int s8_get_current_reading()
{
    uint16_t co2_ppm = 0;
    uint8_t S8_StatusAndCO2Reading[ANSWER_LENGTH_FOR_CO2_AND_STATUS] = {0};
    int status = s8_get_status_and_sensor_reading(S8_StatusAndCO2Reading);
    if (status < 0)
        return -1;

    co2_ppm = S8_StatusAndCO2Reading[10];
    co2_ppm |= (uint16_t)S8_StatusAndCO2Reading[9] << 8;

    if (co2_ppm < MAX_VALID_CO2_VALUE)
        return co2_ppm;

    return S8_INVALID_READING;
}

void s8_get_current_reading_as_string(char* stringPPM)
{
    uint16_t currentReading = s8_get_current_reading();
    if (currentReading != S8_INVALID_READING)
        sprintf(stringPPM,"%u ppm", currentReading);
    else
        strcpy(stringPPM,unknownReadingString);

}
void s8_calibrate_if_requested()
{
    if (s8_is_calibration_button_pressed() == CALIBRATION_BUTTON_PRESSED)
        s8_start_background_calibration();
}