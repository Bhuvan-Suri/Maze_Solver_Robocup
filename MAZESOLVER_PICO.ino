#include "header.h"

void setup() {
  /* Initialize Serial module */
  SerialDebug.begin(115200);

  /* Set motor outputs */
  setMotorIOs();
  /* Initialize I2C module */
  beginWire();

  /* Initialize MPU6050 module */
  beginMpu();

  /* Set pin 7, 13 as output */
  pinMode(BUZZPIN, OUTPUT);
  pinMode(LED13, OUTPUT);

  pinMode(LEFTLED, OUTPUT);
  pinMode(RIGHTLED, OUTPUT);

  pinMode(enc1, INPUT_PULLUP);
  pinMode(enc2, INPUT_PULLUP);

  /* Scan I2C devices (uncomment to check connected I2C device addresses) */
  // testI2C();

  while (!BOOTSEL) {
    Serial.println("ok");
    delay(100);
  }

  mpu.calcOffsets(true, true);  // gyro and accel
  mpu.resetAngleZ();
/* Initializes all proximity sensors */
  initAPDS(FL);
  initAPDS(DL);
  initAPDS(FR);
  initAPDS(DR);

  delay(100);

  /* let the bot rest and wait for user input from proximity sensor > diagonal left */
  waitForUserInput1();

  /* Calculate gyroscope offsets and wait for user input from proximity sensor > diagonal right */
  waitForUserInput2();
  // delayWithSensorUpdate(100);

  encoderPos = 0;
  enableISR();


  /* test the rotation accuracy using diagonal sensors. comment to disable testing. */
  // testRotation();

  /* Start motors */
  for (int i = 0; i < MAXSPEED_move; i++) {
    mpu.update();
    int gyroerror = mpu.getAngleZ() * 2;
    moveMotor(i + gyroerror, i - gyroerror);
    delayMicroseconds(250);
  }

  /* Run main algorithm
  Note: function is blocking*/
  solveMaze();
  for(int i = 0; i < 30; i++){
    for(int i2 = 0; i < 30; i++){
      delete maze[i,i2];
    }
  }

  /* indicate finish state by blinking both leds continously. */
  while (1) {
    state = !state;
    digitalWrite(LEFTLED, state);
    digitalWrite(RIGHTLED, state);
    delay(400);
  }
}

void loop() {
  /* Nothing to do here */
  delay(1);
}
