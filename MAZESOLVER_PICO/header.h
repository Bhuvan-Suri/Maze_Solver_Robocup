/*
 * ENABLE PWM ON PIN0 ARDUINO ZERO
 * FROM
 * { PORTA, 11, PIO_SERCOM, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_11 }, // RX: SERCOM0/PAD[3]
 * TO
 * {PORTA, 11, PIO_DIGITAL, (PIN_ATTR_DIGITAL | PIN_ATTR_PWM | PIN_ATTR_TIMER), ADC_Channel19, PWM1_CH1, TCC1_CH1, EXTERNAL_INT_NONE},
 *
 * add the following to MPU6050_light.h
 * void resetAngleZ(){angleZ=0;};
 */

#define DEBUG

#include <Wire.h>
#include <MPU6050_light.h>
#include "config_pico.h"
#include "maze.h"

bool state = false;

#define SerialDebug Serial

MPU6050 mpu(Wire);

// APDS9960 I2C address
#define APDS9960_ADDRESS 0x39

// APDS9960 register addresses
#define APDS9960_REG_ENABLE 0x80
#define APDS9960_REG_PDATA 0x9C
#define APDS9960_REG_CDATAL 0x94
#define APDS9960_REG_CDATAH 0x95
#define APDS9960_REG_RDATAL 0x96
#define APDS9960_REG_RDATAH 0x97
#define APDS9960_REG_GDATAL 0x98
#define APDS9960_REG_GDATAH 0x99
#define APDS9960_REG_BDATAL 0x9A
#define APDS9960_REG_BDATAH 0x9B
#define APDS9960_REG_CONFIG2 0x90
#define APDS9960_REG_CONFIG3 0x9F
#define APDS9960_REG_CONTROL 0x8F

#define APDS9960_REG_COUNTR 0x8E

volatile int encoderPos = 0;
volatile int lastEncoded = 0;

#define LEFT -1
#define RIGHT 1
#define UTURN 2

#define FRONTWALL 1
#define ADJECENTWALL 2

String heading[] = {"N", "E", "S", "W"};

int HEADING = 0;
// 0 = North
// 1 = East
// 2 = South
// 3 = West

int GLOBAL_X = 0;
int GLOBAL_Y = 0;

int p_move = 0;
int d_move = 0;
int pe_move = 0;

bool prevWallLeft = false;
bool prevWallRight = false;

int fl = 0;
int fr = 0;
int dl = 0;
int dr = 0;

int c = 0;
int r = 0;
int g = 0;
int b = 0;

float fl_filtered = 0;
float fr_filtered = 0;
float dl_filtered = 0;
float dr_filtered = 0;

int prevTile = 0;

void alterLEDstate()
{
  state = !state;
  digitalWrite(LED13, state);
}

void _beep()
{
  digitalWrite(BUZZPIN, HIGH);
  delay(40);
  digitalWrite(BUZZPIN, LOW);
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}