//MotorLib.cpp
//Created by Alex Boyle
#include "Arduino.h"
#include "MotorLib.h"

#define DEFAULT_DUTY_CYCLE  255
#define WHEEL_CIRCUMFERENCE 23.9389360204 //this is in cm!

/* Duty Cycle table!
   0 = 0%
   64 = 25%
   127 = 50%
   191 = 75%
   255 = 100%
*/

//Left wheel pin assignments!
byte leftmotor_pwm;
byte leftmotor_dir_a;
byte leftmotor_dir_b;

//rightwheel pin assignments!
byte rightmotor_pwm;
byte rightmotor_dir_a;
byte rightmotor_dir_b;

//leftWheelInterrupt_ISR variables
volatile byte record_left = 0;
byte interruptPin_left = 2;
volatile int encoder_count_left = 0, desired_count_left = 0;

//rightWheelInterrupt_ISR variables
byte interruptPin_right = 3;
volatile bool record_right = 0;
volatile int encoder_count_right = 0, desired_count_right = 0;

volatile bool done_with_move =1;
//In theory 1185 should be 1 rotation
//But I found 1170 worked a little better
void leftWheelInterrupt_ISR() { //left wheel motor interrupt
	       encoder_count_left++;
        if(encoder_count_left >= desired_count_left){
            done_with_move = 1;
            encoder_count_left = 0;
	        desired_count_left = 0;
            stop_motors();
        }
}

void rightWheelInterrupt_ISR() { //right wheel interrupt
        encoder_count_right++;
        if(encoder_count_right >= desired_count_right){
            record_right = 0;
            encoder_count_right = 0;
	        desired_count_right = 0;
            stop_motors();
        }
}

void motorSetup(byte l_motor_pwm = 13, byte l_motor_dir_a = 22, byte l_motor_dir_b = 24, byte r_motor_pwm = 10, byte r_motor_dir_a = 23, byte r_motor_dir_b = 25){
    pinMode(interruptPin_left, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin_left), leftWheelInterrupt_ISR, RISING); //attaching interrupt for motor encoder

    //pinMode(interruptPin_right, INPUT_PULLUP);
    //attachInterrupt(digitalPinToInterrupt(interruptPin_right), rightWheelInterrupt_ISR, RISING); //attaching interrupt for motor encoder

   //Setup Left motor
   leftmotor_pwm = l_motor_pwm;
   leftmotor_dir_a = l_motor_dir_a;
   leftmotor_dir_b = l_motor_dir_b;

  //Setup Right Motor
   rightmotor_pwm = r_motor_pwm;
   rightmotor_dir_a = r_motor_dir_a;
   rightmotor_dir_b = r_motor_dir_b;

  //These pins set the speed of the motors
  pinMode(leftmotor_pwm, OUTPUT);
  pinMode(rightmotor_pwm, OUTPUT);

  //These pins enable or disable the motors
  pinMode(leftmotor_dir_a, OUTPUT);
  pinMode(leftmotor_dir_b, OUTPUT);
  pinMode(rightmotor_dir_a, OUTPUT);
  pinMode(rightmotor_dir_b, OUTPUT);

  analogWrite(leftmotor_pwm , DEFAULT_DUTY_CYCLE);
  analogWrite(rightmotor_pwm , DEFAULT_DUTY_CYCLE);
}

/*The motor enables basically have three states
   motor_dir_a           motor_dir_b          out
        1                      0                Enabled one direction
        0                      1                Enabled the other direction
        1                      1                Disabled
        0                      0                Disabled
*/

void stop_motors() {
	noInterrupts();
  	digitalWrite(leftmotor_dir_a, 0);
  	digitalWrite(leftmotor_dir_b, 0);

  	digitalWrite(rightmotor_dir_a, 0);
  	digitalWrite(rightmotor_dir_b, 0);

  	done_with_move = 1;
    encoder_count_left = 0;
    encoder_count_right = 0;
    interrupts();
}

void moveFWBW(int ticks, byte motorspeed = DEFAULT_DUTY_CYCLE) {
  noInterrupts();
	done_with_move= 0;
	encoder_count_left = 0;
	if(ticks > 0) desired_count_left = ticks; //forwards
  	else desired_count_left = -1*ticks; //backwards
  interrupts();

  //Set up directions of motors
  if (ticks > 0) {//forwards
    digitalWrite(leftmotor_dir_a, 1);
    digitalWrite(leftmotor_dir_b, 0);

    digitalWrite(rightmotor_dir_a, 0);
    digitalWrite(rightmotor_dir_b, 1);
  } else { //backwards
    digitalWrite(leftmotor_dir_a, 0);
    digitalWrite(leftmotor_dir_b, 1);

    digitalWrite(rightmotor_dir_a, 1);
    digitalWrite(rightmotor_dir_b, 0);
  }
}

//This method is used for a robot that is motionless and wants to turn sharply
//This will be done_with_move by turning one wheel one direction and the other wheel will go the other
void pivot(int ticks, byte speed = DEFAULT_DUTY_CYCLE ) {
  noInterrupts();
    done_with_move= 0;
    encoder_count_left = 0;
    if(ticks < 0) desired_count_left =  -1* ticks;
    else desired_count_left = 1 * ticks;
  interrupts();

  if (ticks < 0) {//pivot right
    digitalWrite(leftmotor_dir_a, 1);
    digitalWrite(leftmotor_dir_b, 0);

    digitalWrite(rightmotor_dir_a, 1);
    digitalWrite(rightmotor_dir_b, 0);
} else {//pivot left
    digitalWrite(leftmotor_dir_a, 0);
    digitalWrite(leftmotor_dir_b, 1);

    digitalWrite(rightmotor_dir_a, 0);
    digitalWrite(rightmotor_dir_b, 1);
  }
}
