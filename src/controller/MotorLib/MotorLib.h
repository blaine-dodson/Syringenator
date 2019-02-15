/*
	MotorLib.h - Library for two motors
	Created by Alex Boyle
*/

#ifndef MotorLib_h
#define MotorLib_h

#include "Arduino.h"

//The wheel assignments are from the perspective of the camera!
//Left wheel pin assignments!


enum movementType{
	forward,
	backward,
	left,
	right,
	pivotLeft,
	pivotRight
};

struct movement {
  movementType move; //look at movementType enum
  byte duration; //How many rotations specifically
  byte direction; // 1 = forward, 0 = backward
};

void leftWheelInterrupt_ISR();
void rightWheelInterrupt_ISR();

void motorSetup(byte l_motor_pwm = 13, byte l_motor_dir_a = 22, byte l_motor_dir_b = 24, byte r_motor_pwm = 10, byte r_motor_dir_a = 23, byte r_motor_dir_b = 25);
void robotGoTo(double x, double y);
void moveRobot(movementType typeMove, double desired_rev, bool move_direction = 1);
void intiateMove(movementType typeMove, double desired_rev, bool move_direction = 1, byte speedBoth = 255);
void stop_motors();
void moveStraight(bool direction, byte motorspeed = 255);
double convertDistanceToRotations(double dist);
//This method is used for a robot that is motionless and wants to turn sharply
//This will be done by turning one wheel one direction and the other wheel will go the other
void pivot(byte speed, bool direction);
void rotate(double angle);
void turn(byte Lspeed , byte Rspeed, bool direction);

#endif
