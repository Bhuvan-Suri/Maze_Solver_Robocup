void wire1_TCA9548A(uint8_t bus) {
  Wire1.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire1.write(1 << bus);          // send byte to select bus
  Wire1.endTransmission();
}

void initColorSensor(bus){
  if (!apds.begin()) {
    wire1_TCA9548A(bus);
    SerialDebug.println("failed to initialize device! Please check your wiring.");
  } else SerialDebug.println("Device initialized!");

  //enable color sensign mode
  apds.enableColor(true);
}

void initColorSensors(){
  initColorSensor(f_C);
  initColorSensor(l_C);
  initColorSensor(r_C);
}

void readColorSensor(bus){
  //wait for color data to be ready
  wire1_TCA9548A(bus)
  while (!apds.colorDataReady()) {
    delay(5);
  }

  //get the data and print the different channels
  apds.getColorData(&r, &g, &b, &c);

  r_filtered = alpha * r_filtered + (1 - alpha) * r;
  g_filtered = alpha * g_filtered + (1 - alpha) * g;
  b_filtered = alpha * b_filtered + (1 - alpha) * b;
  c_filtered = alpha * c_filtered + (1 - alpha) * c;

  SerialDebug.print("r:");
  SerialDebug.print((int)r_filtered);

  SerialDebug.print(",g:");
  SerialDebug.print((int)g_filtered);

  SerialDebug.print(",b:");
  SerialDebug.print((int)b_filtered);

  SerialDebug.print(",c:");
  SerialDebug.print((int)c_filtered);

  // Condition 1: r < b < g and c is roughly 55 -> Red
  if (g<r && abs(b - b) <= 5 && c<75) {
    SerialDebug.println("Red");
  }
  // Condition 2: b < r < g and c is roughly 45 -> Black
  else if (r < g && g < b && c<75) {
    SerialDebug.println("Black");
  }
  // Condition 3: b < r and r roughly equals g and c is roughly 55 -> Green
  else if (r < g && abs(b - g) <= 5 && c<75) {
    SerialDebug.println("Green");
  } else {
    SerialDebug.println("No match");
  }

  delay(5);
}

void readColorSensors(){
  readColorSensor(f_C);
  readColorSensor(l_C);
  readColorSensor(r_C);
}