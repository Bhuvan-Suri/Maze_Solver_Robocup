void beginWire()
{
  Wire.setSCL(1);
  Wire.setSDA(0);
  Wire.begin();
  Wire.setClock(400000);
}

void wire1_TCA9548A(uint8_t bus)
{
  Wire1.beginTransmission(0x70); // TCA9548A address is 0x70
  Wire1.write(1 << bus);         // send byte to select bus
  Wire1.endTransmission();
}

void wire1_writeRegister(uint8_t reg, uint8_t value)
{
  Wire1.beginTransmission(APDS9960_ADDRESS);
  Wire1.write(reg);
  Wire1.write(value);
  Wire1.endTransmission();
}

uint8_t *wire1_readRegister(uint8_t reg, uint8_t len)
{
  static uint8_t data[5]; // Assuming len is always <= 3
  Wire1.beginTransmission(APDS9960_ADDRESS);
  Wire1.write(reg);
  Wire1.endTransmission();

  Wire1.requestFrom(APDS9960_ADDRESS, len);
  while (Wire1.available() < len)
    ;

  for (uint8_t i = 0; i < len; i++)
    data[i] = Wire1.read();
  return data;
}

int readProximity(uint8_t channel)
{
  wire1_TCA9548A(channel);
  uint8_t *proximityData = wire1_readRegister(APDS9960_REG_PDATA, 2);
  int proximity = (proximityData[1] << 8) | proximityData[0];
  return proximity;
}

void initAPDS(uint8_t channel)
{
  wire1_TCA9548A(channel);
  delay(20);
  wire1_writeRegister(APDS9960_REG_ENABLE, (0 << 6) | (1 << 5) | (0 << 4) | (0 << 3) | (1 << 2) | (0 << 1) | 1);
  wire1_writeRegister(APDS9960_REG_CONFIG2, (0 << 7) | (0 << 6) | (0x03 << 4) | 1);
  // wire1_writeRegister(APDS9960_REG_CONFIG3, (0 << 5) | (0 << 3) | (0 << 2) | (0 << 1) | (0 << 0));
  wire1_writeRegister(APDS9960_REG_CONTROL, (0x00 << 6) | (0x03 << 2) | 0x01);
  // wire1_writeRegister(APDS9960_REG_COUNTR, (0x00 << 6));
  delay(50);
}

int detectTurn(bool currDL, bool currDR)
{
  if (currDL != prevWallLeft)
  {
    prevWallLeft = currDL;
    if (!currDL)
      return LEFT;
  }
  else if (currDR != prevWallRight)
  {
    prevWallRight = currDR;
    if (!currDR)
      return RIGHT;
  }
  return 0;
}

void updateWalls(bool _delay)
{
  for (int i = 0; i < 5; i++)
  {
    updateProximitySensors();
    if (_delay)
      delay(5);
    else
      delay(2);
  }
  prevWallLeft = (dl_filtered > leftTurnSense);
  prevWallRight = (dr_filtered > rightTurnSense);
}

void updateProximitySensors()
{
  fm = readProximity(FM);
  dl = readProximity(DL);
  dr = readProximity(DR);

  dl_filtered = alpha * dl_filtered + (1 - alpha) * dl;
  dr_filtered = alpha * dr_filtered + (1 - alpha) * dr;
  fm_filtered = alpha * fm_filtered + (1 - alpha) * fm;

}

void delayWithSensorUpdate(int ms)
{ // only in multiples of 5
  long _curr = millis();
  delay(1);
  while ((millis() - _curr) < ms)
  {
    updateProximitySensors();
    mpu.update();
  }
}

void waitForUserInput1()
{
  while (1)
  {
    updateProximitySensors();
    mpu.update();
    if (dl > 200)
      break;
    alterLEDstate();
    SerialDebug.println("waiting1..");
    delay(50);
  }
  // _beep();
  state = HIGH;
  alterLEDstate();
}

void waitForUserInput2()
{
  while (1)
  {
    updateProximitySensors();
    mpu.update();
    if (dr > 200)
      break;
    alterLEDstate();
    SerialDebug.println("waiting2..");
    delay(100);
  }
  _beep();
  state = HIGH;
  alterLEDstate();
}

void printWalldata()
{
  SerialDebug.print("fl:");
  SerialDebug.print(fl);
  SerialDebug.print(",fr:");
  SerialDebug.print(fr);
  SerialDebug.print(",dl:");
  SerialDebug.print(dl);
  SerialDebug.print(",dr:");
  SerialDebug.println(dr);
}