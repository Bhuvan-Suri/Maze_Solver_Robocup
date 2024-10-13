void beginMpu()
{
  // Wire.begin();
  byte status = mpu.begin(3, 0);
  SerialDebug.print(F("MPU6050 status: "));
  SerialDebug.println((status == 0) ? "OK" : "ERROR");
  while (status != 0)
  {
  } // stop everything if could not connect to MPU6050
}
