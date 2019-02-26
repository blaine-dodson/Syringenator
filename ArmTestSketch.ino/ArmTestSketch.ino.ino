#include <LobotServoController.h>
#include <SoftwareSerial.h>

//Variable Declarations
char angle = 90;
char radius = 19;
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
void grabObject(byte angle, byte radius, byte handAngle){

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
  //servos[1].Position = 500 + (handAngle/90)*381;
  //All case statements are degrees with 90 being parallel to the robot
  switch(handAngle){
    case 45:
      servos[1].Position = 300;
      break;
    case 90: 
      servos[1].Position = 500;
      break;
    case 135: 
      servos[1].Position = 700;
      break;
    default:
      servos[1].Position = 100;
  }
  //xArm.moveServo(2, servos[1].Position, 1000);
  //Set hand to default narrow open postion ID:1 which is 3cm wide
  servos[0].ID = 1;
  servos[0].Position = 500;
  //Make sure hand is in position before moving to the radius and theta
  //xArm.moveServo(1, 500, 1000);
  //Calculate the position for the other 3 servos that are controlled by the radius
  //ID:3-5
  //This assumes that numbers given are based off of the center of the arm
  //10cm is the reach of the xArm from the front of the robot
  //Calculate (alpha) the angle of the base corners of the isoseles triangle
//  //9.5cm is the length of arm between servos
//  double alpha = acos(0.5*(radius/9.5));
//  servos[2].ID = 3;
//  servos[2].Position = (alpha/180.0)*1000 + 128;
//  servos[3].ID = 4;
//  if(radius == 19)
//    servos[3].Position = 489;
//  else
//    servos[3].Position = ((180-(2*alpha))/180)*1000 + 489;
//  servos[4].ID = 5;
//  servos[4].Position = (alpha/180)*1000 + 103;
  //All case statements are in cm
  switch(radius){
    case 19:
      servos[2].ID = 3;
      servos[2].Position = 114;
      servos[3].ID = 4;
      servos[3].Position = 481;
      servos[4].ID = 5;
      servos[4].Position = 102;
      break;
    case 18:
      servos[2].ID = 3;
      servos[2].Position = 208;
      servos[3].ID = 4;
      servos[3].Position = 658;
      servos[4].ID = 5;
      servos[4].Position = 184;
      break;
    case 17:
      servos[2].ID = 3;
      servos[2].Position = 264;
      servos[3].ID = 4;
      servos[3].Position = 752;
      servos[4].ID = 5;
      servos[4].Position = 231;
      break;
    case 16:
      servos[2].ID = 3;
      servos[2].Position = 283;
      servos[3].ID = 4;
      servos[3].Position = 796;
      servos[4].ID = 5;
      servos[4].Position = 252;
      break;
    case 15:
      servos[2].ID = 3;
      servos[2].Position = 291;
      servos[3].ID = 4;
      servos[3].Position = 816;
      servos[4].ID = 5;
      servos[4].Position = 261;
      break;
    case 14:
      servos[2].ID = 3;
      servos[2].Position = 308;
      servos[3].ID = 4;
      servos[3].Position = 853;
      servos[4].ID = 5;
      servos[4].Position = 278;
      break;
    case 13:
      servos[2].ID = 3;
      servos[2].Position = 325;
      servos[3].ID = 4;
      servos[3].Position = 885;
      servos[4].ID = 5;
      servos[4].Position = 293;
      break;
    case 12:
      servos[2].ID = 3;
      servos[2].Position = 342;
      servos[3].ID = 4;
      servos[3].Position = 914;
      servos[4].ID = 5;
      servos[4].Position = 309;
      break;
     default:
     //Default is 19 cm
      servos[2].ID = 3;
      servos[2].Position = 114;
      servos[3].ID = 4;
      servos[3].Position = 481;
      servos[4].ID = 5;
      servos[4].Position = 102;
      break;
  }

  //Move servos to the positions given
  xArm.moveServos(servos, 6, 2000);
  delay(2000);
  //Close the grip on the needle
  xArm.moveServo(1, 1000, 2000);
  delay(2000);
  //Drop the needle
  //Run action group 1, 1 time
  xArm.runActionGroup(1, 1);
  delay(3000);
  //Return to default resting position
  servos[0].Position = 498;
  servos[1].Position = 478;
  servos[2].Position = 46;
  servos[3].Position = 913;
  servos[4].Position = 879;
  servos[5].Position = 501;
  xArm.moveServos(servos, 6, 2000);
  delay(2000);
}
