#include <SensorLib.h>
#include <MotorLib.h>
#include <Syringenator.hpp>
#include <Constants.hpp>
#include <LobotServoController.h>

char buffer[4];
byte newCommand = 0;

void clearBuffer() {
  for ( int i = 0; i < sizeof(buffer);  ++i )
    buffer[i] = (char)0;
}

void setup() {
  motorSetup();
  setupSensor_ISR(); //use this to set up timer interrupts for line sensors and distance sensors
  Serial.begin(9600); //For communication with the pi
  Serial1.begin(9600); //For communication with the xarm
  Serial.flush();
  clearBuffer();

  Serial.print(ARDUINO_STATUS_READY);//Let the pi know we are initialized and ready for commands
}

void loop() {
  if (newCommand) { //set to 1 in the serial event function
    parseCommand(buffer);
    newCommand = 0;
  }
  delay(50);
}

//easiest to send serial data as ascii strings to python...
void parseCommand(char input[ ]) { //commands sent to the arduino from the raspberry pi
  switch (input[0]) {
    case ARDUINO_ROTATE:
      moveRotate(input[1],1);
      break;
    case ARDUINO_MOVE:
      moveStraight(input[1],input[2],1);
      break;
    case ARDUINO_LINE_FOLLOW:
      moveLineFollow();
      break;
    case ARDUINO_ARM_PICKUP:
      grabObject(input[1], input[2], input[3]);
      break;
  }
   while (!isDoneCommand(1));
  Serial.print(ARDUINO_STATUS_READY);
  clearBuffer();
}

void serialEvent() {//an interrupt handler for the serial communication
  int i = 0;
  while (Serial.available()) {
    // get the new byte:
    buffer[i] = (char)Serial.read();
    i++;
  }
  newCommand = 1;
}
