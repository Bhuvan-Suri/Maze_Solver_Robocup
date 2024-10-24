#define SerialDebug SerialUSB
#include "Adafruit_APDS9960.h"
Adafruit_APDS9960 apds;

#define r_C 2
#define l_C 3

const float alpha = 0.65;

//Filtered RGBC values
float r_filtered = 0;
float g_filtered = 0;
float b_filtered = 0;
float c_filtered = 0;

//Unfiltered RGBC values
uint16_t r, g, b, c;