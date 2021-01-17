#pragma once

#define S8_INVALID_READING    -1

void s8_init();
int s8_get_current_reading();
void s8_calibrate_if_requested();
void s8_get_current_reading_as_string(char* stringPPM);
