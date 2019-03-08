//Created by Alex Boyle

#include "Arduino.h"
#include <MedianFilter.h>
#include <SharpDistSensor.h>

#define OBJ_AVOID_DISTANCE 600

byte line_right_pin = 29 ,line_left_pin= 31;

SharpDistSensor back_right_sensor = NULL;
SharpDistSensor back_left_sensor = NULL;
SharpDistSensor front_right_sensor = NULL;
SharpDistSensor front_left_sensor = NULL;

void setupDistanceSensors(byte front_right_pin = A14,byte front_left_pin = A15,byte back_right_pin = A13, byte back_left_pin = A12, byte medianFilterWindowSize = 5){
	SharpDistSensor back_right_sensor(back_right_pin, medianFilterWindowSize);
	SharpDistSensor back_left_sensor(back_left_pin, medianFilterWindowSize);
	SharpDistSensor front_right_sensor(front_right_pin, medianFilterWindowSize);
	SharpDistSensor front_left_sensor(front_left_pin, medianFilterWindowSize);
}

int readFrontSensors(){
	unsigned int dist_fl =  front_left_sensor.getDist();
  	unsigned int dist_fr = front_right_sensor.getDist();

  if ( dist_fl < OBJ_AVOID_DISTANCE || dist_fr < OBJ_AVOID_DISTANCE) { //There may be something in front!

    if (dist_fl < dist_fr) {
     	return 1;
    }
    else if (dist_fl > dist_fr) {
	    return -1;
    }
  }
  return 0;
}

int readBackSensors(){
	unsigned int dist_bl =  back_left_sensor.getDist();
  	unsigned int dist_br = back_right_sensor.getDist();

  if ( dist_bl < OBJ_AVOID_DISTANCE || dist_br < OBJ_AVOID_DISTANCE) { //There may be something in front!

    if (dist_bl < dist_br)  return 2;
    else if (dist_bl > dist_br)  return -2;

  }
  return 0;
}



//Line sensors
void setupLineSensors(byte in_left_wheel_pin = 31, byte in_right_wheel_pin = 29){
	line_right_pin = in_right_wheel_pin;
	line_left_pin = in_left_wheel_pin;
}

/*
	readLine functions taken from the http://bildr.org/2011/06/qre1113-arduino/ which was
 	linked from https://www.sparkfun.com/products/9454
*/

int readLine_right() {
//  noInterrupts();
  pinMode(line_right_pin, OUTPUT);
  digitalWrite(line_right_pin, HIGH);
  delayMicroseconds(10);
  pinMode(line_right_pin, INPUT);

  long time = micros();

  while (digitalRead(line_right_pin) == HIGH && micros() - time < 3000);
  int diff = micros() - time;
 //interrupts();
  return diff;
}

int readLine_left() {
  //noInterrupts();
  pinMode(line_left_pin, OUTPUT);
  digitalWrite(line_left_pin, HIGH);
  delayMicroseconds(10);
  pinMode(line_left_pin, INPUT);

  long time = micros();

  while (digitalRead(line_left_pin) == HIGH && micros() - time < 3000);
  int diff = micros() - time;
 //interrupts();
  return diff;
}
