//Arm Library Header File

#ifndef ArmLib_h
#define ArmLib_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "LobotServoController.h"

//Grabs the object AKA needle
void grabObject(double angle, double radius, double handAngle);

//Takes an angle and moves the arm to that angle
void moveToAngle(double angle, LobotServoController &xArm);

//Takes a radius and moves the arm to that location
void pickUpObject(double radius, LobotServoController &xArm);

//Takes a hand angle and moves the hand to that angle orientation
void moveToHandAngle(double handAngle, LobotServoController &xArm);

//Returns the servo motors positions in an array
int * readServoPositions(LobotServoController &xArm);

#endif