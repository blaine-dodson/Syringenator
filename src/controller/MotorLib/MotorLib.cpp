//MotorLib.cpp
//Created by Alex Boyle
#include "Arduino.h"
#include "MotorLib.h"
#include "Math.h"

#define DEFAULT_DUTY_CYCLE  255
#define WHEEL_CIRCUMFERENCE 23.9389360204 //this is in cm!
#define FULL90ROT 1.15
/* Duty Cycle table!
   0 = 0%
   64 = 25%
   127 = 50%
   191 = 75%
   255 = 100%
*/

//leftWheelInterrupt_ISR
byte record = 0;
byte interruptPin = 2;
volatile unsigned int encoder_count = 0, revolutions_count = 0;
volatile double desiredRevolutions;

//rightWheelInterrupt_ISR
byte interruptPin_2 = 3;
byte record_2 = 0;
volatile unsigned int encoder_count_2 = 0, revolutions_count_2 = 0;
volatile double desiredRevolutions_2;

//Left wheel pin assignment variables
byte leftmotor_pwm;
byte leftmotor_dir_a;
byte leftmotor_dir_b;

//rightwheel pin assignments!
byte rightmotor_pwm;
byte rightmotor_dir_a;
byte rightmotor_dir_b;

queue <movmement> movesQueue; //holds upcoming moves

void leftWheelInterrupt_ISR() { //left wheel motor interrupt
    encoder_count++;
    if(record){
        if (encoder_count >= 1170 ) { //In theory 1185 should be 1 rotation
            encoder_count = 0;        //But I found 1170 worked a little better
            revolutions_count++;
        }
        if(revolutions_count >= desiredRevolutions){
            record = 0;
            encoder_count = 0;
            revolutions_count = 0;

            stop_motors();
            //check queue
            movement temp = movesQueue.pop();
            if(temp != null)
                intiateMove(temp.move,temp.duration,temp.direction);
        }
    }
}

void rightWheelInterrupt_ISR() { //right wheel interrupt
    if(record_2){
        encoder_count_2++;
        if (encoder_count_2 >= 1170 ) {
            encoder_count_2 = 0;
            revolutions_count_2++;
        }
        if(revolutions_count_2 >= desiredRevolutions_2){
            record_2 = 0;
            encoder_count_2 = 0;
            revolutions_count_2 = 0;

            stop_motors();
            movement temp = movesQueue.pop();
            if(temp != null)
                initiateMove(temp.move,temp.duration,temp.direction);
        }
    }
}

double convertDistanceToRotations(double dist){//The wheel radius is ~1.5 inches so ~3.81 cm
    //using the relation 2(pi)r = circumference...
    //2(3.14159265359)(3.81cm) = 23.9389360204 cm or 9.424777960787402 in per revolution
    return dist/WHEEL_CIRCUMFERENCE;
}

void motorSetup(byte l_motor_pwm = 13, byte l_motor_dir_a = 22, byte l_motor_dir_b = 24, byte r_motor_pwm = 10, byte r_motor_dir_a = 23, byte r_motor_dir_b = 25){

    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), leftWheelInterrupt_ISR, RISING); //attaching interrupt for motor encoder

    pinMode(interruptPin_2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin_2), rightWheelInterrupt_ISR, RISING); //attaching interrupt for motor encoder

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

//this is not working!
void robotGoTo(double x, double y){//inputs are in cm
    //do initial calculations
    //get angle
    double angle = atan2(y,x);
    //rotate robot
    rotate(angle);

    double distance = sqrt(pow(x,2)+pow(y,2));
    double rotations = convertDistanceToRotations(distance);
    while(record != 0 || record_2 != 0);//wait for signal
    moveRobot(0,rotations);//just did 0 in place of the enum forward
}

void moveRobot(movementType typeMove, double desired_rev, bool move_direction = 1){
    //enqueue
    movement temp;
    temp.move = typeMove;
    temp.duration = desired_rev;
    temp.direction = move_direction;
    movesQueue.push(temp);
}

void initiateMove(movementType typeMove, double desired_rev, bool move_direction = 1, byte speedBoth = 255) {
 //Since we need to modify volatile variables count and revolutions we need to turn off interrupts
  switch (typeMove) {
    case forward:
      moveStraight(speedBoth, 1,desired_rev);
      break;

    case backward:
      moveStraight(speedBoth, 0,desired_rev);
      break;

    case left:
      turn(125, 255,move_direction,desired_rev);
      break;

    case right:
      turn(255, 125,move_direction,desired_rev);
      break;

    case pivotLeft:
      pivot(255, 1 ,desired_rev);
      break;

    case pivotRight:
      pivot(255,  0, desired_rev);
      break;

    default:
      break;
  }
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
    record = 0;
    record_2 = 0;
    desiredRevolutions = 0;
    encoder_count = 0;
    revolutions_count = 0;

    desiredRevolutions_2 = 0;
    encoder_count_2 = 0;
    revolutions_count_2 = 0;
    interrupts();
  digitalWrite(leftmotor_dir_a, 0);
  digitalWrite(leftmotor_dir_b, 0);

  digitalWrite(rightmotor_dir_a, 0);
  digitalWrite(rightmotor_dir_b, 0);
}

void moveStraight(bool direction, byte motorspeed = 255, double desired_rev) {
  noInterrupts();
  record = 0;
  desiredRevolutions = desired_rev;
  interrupts();

  analogWrite(leftmotor_pwm , motorspeed);
  analogWrite(rightmotor_pwm , motorspeed);

  //Set up directions of motors
  if (direction) {//forwards
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
//This will be done by turning one wheel one direction and the other wheel will go the other
void pivot(byte speed, bool direction, double desired_rev) {

  noInterrupts();
    record = 0;
    desiredRevolutions = desired_rev;
  interrupts();

  analogWrite(leftmotor_pwm , speed); //Duty cycle is 50% so 50% max speed of the motors
  analogWrite(rightmotor_pwm , speed); //Duty cycle is 50% so 50% max speed of the motors

  //dont know exactly what direction will do what yet... need to test...
  if (direction) {
    digitalWrite(leftmotor_dir_a, 1);
    digitalWrite(leftmotor_dir_b, 0);

    digitalWrite(rightmotor_dir_a, 1);
    digitalWrite(rightmotor_dir_b, 0);
} else {
    digitalWrite(leftmotor_dir_a, 0);
    digitalWrite(leftmotor_dir_b, 1);

    digitalWrite(rightmotor_dir_a, 0);
    digitalWrite(rightmotor_dir_b, 1);
  }
}

/*
This is an abstraction of pivot function, input the amount of degrees of rotations
This will probably be used in conjunction with the moveRobot function. With given
x and y coordinates the robot will need to rotate a given amount to face the position
directly then move forward towards it.
This may not be the best way to do it. But its the first method I will try
the simplest method first - ADB
*/

void moveRotate(double angle){
    //convert angle to the appropriate wheel rotation amount
    //Basically we will get an
    double desired_rev;
    byte turnDirection;
    if(angle <  0){ //pivot left
         desired_rev = (-1* angle/(PI/2)) * FULL90ROT;
         turnDirection = 0;
    }
    else{//pivot right
        desired_rev = (1 * angle/(PI/2)) * FULL90ROT;
        turnDirection = 1
    }
    pivot(DEFAULT_DUTY_CYCLE,DEFAULT_DUTY_CYCLE, turnDirection, desired_rev);

}

void turn(byte Lspeed , byte Rspeed, bool direction, double desired_rev) {

    noInterrupts();
        //TODO:Implement actual amount for turning
    interrupts();

  analogWrite(leftmotor_pwm , Lspeed); //Duty cycle is 50% so 50% max speed of the motors
  analogWrite(rightmotor_pwm , Rspeed); //Duty cycle is 50% so 50% max speed of the motors

  //Trying to implement turning for forward and backward turning
  if (direction) {//if the robot is already moving forward
    digitalWrite(leftmotor_dir_a, 1);
    digitalWrite(leftmotor_dir_b, 0);

    digitalWrite(rightmotor_dir_a, 0);
    digitalWrite(rightmotor_dir_b, 1);
  } else { //if the robot is already moving backwards
    digitalWrite(leftmotor_dir_a, 0);
    digitalWrite(leftmotor_dir_b, 1);

    digitalWrite(rightmotor_dir_a, 1);
    digitalWrite(rightmotor_dir_b, 0);
  }
}
