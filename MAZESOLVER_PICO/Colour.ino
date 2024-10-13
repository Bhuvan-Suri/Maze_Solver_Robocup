void initAPDScolour(uint8_t channel)
{
  TCA9548A(channel);
  delay(20);
  writeRegister(APDS9960_REG_ENABLE, (0 << 6) | (0 << 5) | (1 << 4) | (0 << 3) | (1 << 2) | (1 << 1) | 0); 
  // Bit 6 to 0 Disable Gesture
  // Bit 5 to 0 Disable Proximity Interrupt
  // Bit 4 to 1 Enable ALS Interrupt 
  // Bit 3 to 0 Disable Wait Enable
  // Bit 2 to 1 Enable ALS
  // Bit 1 to 1 Enable ALS Power 
  // Bit 0 to 0 Disable Proximity Power
  writeRegister(APDS9960_REG_CONFIG2, (0 << 7) | (0 << 6) | (0x00 << 4) | 1);
  // Bit 7 to 0 Disable Proximity Saturation Interrupt
  // Bit 6 to 0 Disable Diode Saturation Interrupt
  // Bit 4 to 0x00 Sets Proximity LED Boost to 0%
  // Bit 0 to 1 Reserved must be set to 1
  // writeRegister(APDS9960_REG_CONFIG3, (0 << 5) | (0 << 3) | (0 << 2) | (0 << 1) | (0 << 0));
  writeRegister(APDS9960_REG_CONTROL, (0x00 << 6) | (0x00 << 2) | 0x03);
  // writeRegister(APDS9960_REG_COUNTR, (0x00 << 6));
  // Bit 6 to 0x00 Sets LED Current to 100 Mah
  // Bit 2 to 0x03 Sets Proximity Gain to 1
  // Bit 0 to 0x03 Sets ALS Gain to 64x
  delay(50);
}
int readColour(uint8_t channel, uint8_t reg1, uint8_t reg2)
{
  TCA9548A(channel);
  uint8_t *ColourData1 = readRegister(reg1, 2);
  uint8_t *ColourData2 = readRegister(reg2, 2);
  int Colour = (ColourData1[1] << 24) | (ColourData1[0] << 16) | (ColourData2[1] << 8) | ColourData2[0];
  int RGBVal = Colour/16843009;
  return RGBVal;
}

void updateColour(){
  c = readColour(CRGB, APDS9960_REG_CDATAL, APDS9960_REG_CDATAH);
  r = readColour(CRGB, APDS9960_REG_RDATAL, APDS9960_REG_RDATAH);
  g = readColour(CRGB, APDS9960_REG_GDATAL, APDS9960_REG_GDATAH);
  b = readColour(CRGB, APDS9960_REG_BDATAL, APDS9960_REG_BDATAH);
}