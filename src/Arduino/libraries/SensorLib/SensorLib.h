/*
	SensorLib.h - function library for line sensors and distance sensors
	Created by Alex Boyle
*/

#ifndef SensorLib_h
#define SensorLib_h

#include "Arduino.h"
#include <MedianFilter.h>
#include <SharpDistSensor.h>

//Distance Senors
void setupDistanceSensors(byte front_right_pin = A14,byte front_left_pin = A15,byte back_right_pin = A13, byte back_left_pin = A12, byte medianFilterWindowSize = 5);

//compare the two sensor values and return one of the following:
/*
  	-1  -> object detected withing min distance allowed, but left sensor is less than right sensor
	 0	-> no objects detected in allowed min distance
	 1	-> object detected withing min distance allowed, but left sensor is greater than right sensor
*/
int readFrontSensors();
int readBackSensors();

//Line Sensor Functions
void setupLineSensors(byte leftwheel_pin = 31, byte rightwheel_pin = 29);

//The line functions below are polling functions which at the moment do use the
// delay function...
int readLine_right();
int readLine_left();


#endif
