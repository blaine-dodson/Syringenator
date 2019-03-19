/*
	MotorLib.h - Library for two motors
	Created by Alex Boyle
*/

#ifndef MotorLib_h
#define MotorLib_h

#include "Arduino.h"

extern volatile bool done_with_move;

void motorSetup(byte l_motor_pwm = 13, byte l_motor_dir_a = 22, byte l_motor_dir_b = 24, byte r_motor_pwm = 10, byte r_motor_dir_a = 23, byte r_motor_dir_b = 25);

void leftWheelInterrupt_ISR();
void rightWheelInterrupt_ISR();

void stop_motors();
void moveFWBW(int ticks, byte motorspeed = 255);

//This will do a conversion of the input angle to the arguments the pivot function needs
void pivot(int ticks,byte speed =255);
void pivot_off_left();
void pivot_off_right();

#endif
