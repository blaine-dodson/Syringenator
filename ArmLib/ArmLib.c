//Arm Library C Functions

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "LobotServoController.h"

//Grabs the object AKA needle
void grabObject(double angle, double radius, double handAngle, LobotServoController &xArm){
	//Move arm to angle
	moveToAngle(angle, xArm);
	//Move arm to hand angle orientation
	moveToHandAngle(handAngle, xArm);
	//Move arm to radius and pick up and dispose
	pickUpObject(radius, xArm);
}

//Takes an angle and moves the arm to that angle
//Only affects Servo ID:6
void moveToAngle(double angle, LobotServoController &xArm){
	//Define position for Servo ID:6
	int servo6;

	//Convert angle to a position for Servo ID:6


	//move No.1 servo in 1000ms to servo1 position
	xArm.moveServo(6,servo6,1000);
}

//Takes a radius and moves the arm to that location
void pickUpObject(double radius, LobotServoController &xArm){
	//Read current servo motor positions
	int * servoPos = readServoPositions();
	//Define all servo motor positions
	int servo1, servo2, servo3, servo4, servo5, servo6;
	servo1 = *(servoPos);
	servo2 = *(servoPos + 1);
	servo6 = *(servoPos + 5);


	//Move arm to grab object
	xArm.moveServos(6, 1000, 1, servo1, 2, servo2, 3, servo3, 4, servo4, 5, servo5, 6, servo6);

	//Close hand, Servo ID:1 should be 675 for all the way closed
	xArm.moveServos(6, 1000, 1, 675, 2, servo2, 3, servo3, 4, servo4, 5, servo5, 6, servo6);

	//Action Group to Dispose of object and return to resting position
	//First argument is the action group number, which in this case is action group 0
	xArm.runActionGroup(0,0);
}

//Takes a hand angle and moves the hand to that angle orientation
//Only affects Servo ID:2 for rotation and Servo ID:1 for opening the claw
void moveToHandAngle(double handAngle, LobotServoController &xArm){
	//Define the servo positions
	int servo1, servo2;
	//This is the position of the claw being open as wide as possible
	servo2 = 125;

	//Convert handAngle to a position for Servo ID:2


	//move No.1 servo in 1000ms to servo1 position
	xArm.moveServo(1,servo1,1000);
	//move No.2 servo in 1000ms to servo2 position
	xArm.moveServo(2,servo2,1000);
}

//Returns the servo motors positions in an array
//6 Servo motors in total
int * readServoPositions(LobotServoController xArm){
	int * servoPos;

	return servoPos;
}