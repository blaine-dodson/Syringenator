/**	@file controller.ino
 	The Arduino sketch.
*/
#include <SensorLib.h>
#include <MotorLib.h>
#include <Syringenator.hpp>
#include <Constants.hpp>
#include <LobotServoController.h>

char buffer[16];
byte newCommand = 0;

void clearBuffer() {
  for ( int i = 0; i < sizeof(buffer);  ++i )
    buffer[i] = (char)0;
}

void setup() {
  motorSetup();
  setupSensor_ISR(); //use this to set up timer interrupts for line sensors and distance sensors
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.flush();
  clearBuffer();
}

void loop() {
  if (newCommand) {
    parseCommand(buffer);
    newCommand = 0;
  }
  delay(50);
}

void parseCommand(char input[ ]) { //commands sent to the arduino from the raspberry pi
  switch (input[0]) {
    case ARDUINO_ROTATE:
      moveRotate(input[1]);
      while (!isDoneCommand(1));
      Serial.print(ARDUINO_STATUS_READY);
      break;
    case ARDUINO_MOVE:
      moveStraight(input[1]);
      while (!isDoneCommand(1));
      Serial.print(ARDUINO_STATUS_READY);
      break;
    case ARDUINO_LINE_FOLLOW:
      moveLineFollow();
      Serial.print(ARDUINO_STATUS_READY);
      break;
    case 3:
      stopLineFollow();
      Serial.print(ARDUINO_STATUS_READY);
      break;
    case ARDUINO_ARM_PARK :
    case ARDUINO_ARM_DISPOSE :
    case ARDUINO_ARM_PICKUP:
      grabObject(input[1], input[2], input[3]);
      Serial.print(1);
      break;

  }
  clearBuffer();
}


void serialEvent() {
  int i = 0;
  while (Serial.available()) {
    // get the new byte:
    buffer[i] = (char)Serial.read();
    i++;
  }
  newCommand = 1;
}
