/*
   xArm Robotic Arm Example Sketch
   Written by: Austin Reynolds
   1/9/19
   TCES 460 Embedded Systems Design
   University of Washington - Tacoma
   
   Uses any USB Serial input to advance to next xArm command
   Uses Serial1 on Arduino Mega-compatible board to control xArm
*/

#include <LobotServoController.h>

LobotServoController xArm(Serial1);

void waitForSerial() {
  while(Serial.available() == 0) {}
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  while (!Serial || !Serial1) {}

  xArm.moveServos(6, 1000, 1, 500, 2, 500, 3, 500, 4, 500, 5, 500, 6, 500);
  delay(1000);
  Serial.println("Reset Servos");
  
  waitForSerial(); //wait for serial input to execute next command

  //moveServo arguments - Servo ID, Position, Time(ms)
  xArm.moveServo(5, 350, 1000);
  delay(1000);
  Serial.println("moveServo");

  waitForSerial();

  //moveServos arguments - Number of servos, Time(ms), Servo ID, Position, Servo ID, Position, ...
  xArm.moveServos(6, 1000, 1, 600, 2, 400, 3, 600, 4, 400, 5, 600, 6, 400);
  delay(1000);
  xArm.moveServos(6, 1000, 1, 400, 2, 600, 3, 400, 4, 600, 5, 400, 6, 600);
  delay(1000);
  Serial.println("moveServos - list");

  waitForSerial();

  //moveServos arguments (array form) - Array of struct LobotServo, Number of servos, Time(ms)
  LobotServo servos[6];
  for (int i = 0; i < 6; i++ ) {
    servos[i].ID = i + 1;
    servos[i].Position = 500;
  }
  xArm.moveServos(servos, 6, 1000);
  Serial.println("moveServos - array");

  waitForSerial();

  //runActionGroup arguments - Action Group Number, Number of times to run (0 is infinite)
  xArm.runActionGroup(0, 3);
  while(!xArm.waitForStopping(10000)){}

  Serial.println("Finished");
}

void loop() {}
