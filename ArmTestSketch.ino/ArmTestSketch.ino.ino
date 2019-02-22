#include <LobotServoController.h>
#include <SoftwareSerial.h>

//Variable Declarations
char angle = 90;
char radius = 13;
char handAngle = 90;
uint16_t *servoPos;

//#define rxPin 19
//#define txPin 18
//
//SoftwareSerial mySerial(rxPin, txPin);
LobotServoController xArm(Serial1);

void waitForSerial() {
  while(Serial1.available() == 0) {}
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
//  mySerial.begin(9600);
  while (!Serial1 || !Serial) {}
  grabObject(angle, radius, handAngle);
}

void loop() {
}

//Grabs the object AKA needle
void grabObject(char angle, char radius, char handAngle){

  LobotServo servos[6];
  //Calculate position based off the angle for ID:6
  //756 is a constant position for the ratio of 180 degrees to position number
  servos[5].ID = 6;
  servos[5].Position = (angle/180.0)*1000;
  Serial.println(servos[5].Position);
  //Calculate position based off the handAngle for ID:2
  //881 is a constant position for the ratio of 90 degrees to position number
  //90 being Position 500 (the default position)
  servos[1].ID = 2;
  servos[1].Position = 500 + (handAngle/90)*381;
  //Set hand to default narrow open postion ID:1 
  servos[0].ID = 1;
  servos[0].Position = 500;
  //Calculate the position for the other 3 servos that are controlled by the radius
  //ID:3-5
  //This assumes that numbers given are based off of the center of the arm
  //10cm is the reach of the xArm from the front of the robot
  //Calculate (alpha) the angle of the base corners of the isoseles triangle
  //9.5cm is the length of arm between servos
  double alpha = acos(0.5*(radius/9.5));
  servos[2].ID = 3;
  servos[2].Position = (alpha/180.0)*1000 + 128;
  servos[3].ID = 4;
  if(radius == 19)
    servos[3].Position = 489;
  else
    servos[3].Position = ((180-(2*alpha))/180)*1000 + 489;
  servos[4].ID = 5;
  servos[4].Position = (alpha/180)*1000 + 103;
  
  xArm.moveServos(servos, 6, 1000);
  //return servos;
}
