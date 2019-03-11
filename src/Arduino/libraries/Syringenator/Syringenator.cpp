/**	@file Syringenator.hpp
 *
 *	Arduino controller code
 *	--ABD
 *
 *	@copyright Copyright &copy; 2019 by the authors. All rights reserved.
 */

#include "Syringenator.hpp"

#include "MotorLib.h"
#include "SensorLib.h"
#include "Arduino.h"

//movements array for saving movements that will be used for dead reckoning...
roboMove move_log [LOGSIZE];
int move_log_size = 0;

#define LINE_TIMER_LIMIT 500
#define MOVEMENT_SCALAR 125//229
//variables for the interrupts
volatile bool readDirection = 1; //forwards = 1, back is 0
volatile bool follow_or_obj = 1; //line follow = 0, object detection = 1
volatile int distanceSensTriggered = 0; //holds flag condition from sensor readings
volatile unsigned int left = 100, right = 100;
volatile byte sensorFlag;
volatile unsigned int line_follow_count = 0;

/******************************************************************************/
//                         INTERRUPT SERVICE ROUTINES
/******************************************************************************/


/**	A function to respond to a line detector being triggered.
 *	The line detectors are mounted forward and inboard of the wheels. This function
 *	needs to reorient the robot to clear the sensor, but also to prevent the line
 *	from being hit again.
 *
 *	The simplest way to do this is to rotate the opposite wheel forward until the
 *	sensor clears. Because the sensor is forward of the wheel it will rotate away
 *	from the line as the opposite wheel moves forward. This should work as long as
 *	the curvature of the line is not too great.
 *
 *	This may need to be two routines, one for each sensor
 *	--ABD
 */

/**	A function to respond to a detected obstacle while under locamotion.
 *	There may be two cases to handle: whether we are line following, or aproaching.
 *	If we are line following we need to ensure that we don't lose the line while
 *	avoiding the obstacle.
 *
 *	This may need to be multiple routines, one for each sensor
 *	--ABD
 */


int setupSensor_ISR(){
      noInterrupts();
      TCCR3A = 0;      // clear the timer-counter control registers A and B
      TCCR3B = 0;
      TCNT3  = 0;      // clear the actual counter

      TCCR3B |= (1 << WGM32);   // this sets CTC mode
      TCCR3B |= (1 << CS32);    // this selects the 256 prescaler  (see ATMega2560Datasheet pg 161)
      TIMSK3 |= (1 << OCIE3A);  // this enables the timer compare interrupt

      OCR3A = 5000;            // compare match register for 2 Hz operation (16MHz/256/2Hz)
      interrupts();
}

//If you don't want to use either of the sensors, turn off the ISR with stopSensor_ISR
ISR(TIMER3_COMPA_vect){//Obstacle Detection routine
    if(follow_or_obj){
        if(readDirection){
             sensorFlag = readFrontSensors();
         }
        else {
            sensorFlag = readBackSensors();
        }
        if(sensorFlag != 0) distanceSensTriggered = sensorFlag;
    }else{
            line_follow_count++;
            stopSensor_ISR();
            if(line_follow_count >= LINE_FOLLOW_TIME){
                line_follow_count = 0;
                done_with_command = 1;
                stop_motors();
                return;
            }else{
                readLines();
                startSensor_ISR();
        }
    }
}

void stopSensor_ISR(void){
    noInterrupts();
    TIMSK3 = 0;
    interrupts();
}

void startSensor_ISR(void){
    noInterrupts();
    TIMSK3 |= (1 << OCIE3A);
    interrupts();
}

void switchSensorMode(bool mode){// 1 for obstacle, 0 for line sensor
    noInterrupts();
    follow_or_obj = mode;
    interrupts();
}



int isDoneCommand(int type_command){
    noInterrupts();
    bool motor_move = done_with_move;
    bool temp = done_with_command;
    interrupts();
    if(type_command == 1)
        return motor_move;
    else
        return temp;
}

bool isReady(void){
	noInterrupts();
	bool motor_move = done_with_move;
	bool temp = done_with_command;
	interrupts();
	return motor_move && temp;
}


/******************************************************************************/
//                              LOCAMOTION CONTROL
/******************************************************************************/


/**	Rotate the robot around central axis
 *	rotate by running both motors at the same speed in opposite directions
 *
 *	@param ticks sign indicates direction of rotation: positive is rotation to
 *	the right. magnitude indicates the number of encoder ticks on each motor.
 */
void moveRotate(byte direction, byte angle, bool mode = 0){//expecting input to be from -90 to 90 degrees

    //else we are in line follow mode already...
    float temp;
    int desired_ticks;

    if(direction == ARDUINO_RIGHT){//change input to be able to turn right
        temp = angle /90.0;
        desired_ticks = temp * FULL90ROT_RIGHT; //convert angle to the appropriate wheel rotation amount
    }else{
        temp = angle /90.0;
        desired_ticks = -1* temp * FULL90ROT_LEFT; //convert angle to the appropriate wheel rotation amount
    }
    if(mode){//for obstacle avoidance, used when we go away from the line to pick up a target
        startSensor_ISR();
        noInterrupts();
        readDirection = 0;//read back sensors while pivoting
        follow_or_obj = 1;
        interrupts();
        logMove(0,desired_ticks);
   }
     pivot(desired_ticks);
}


//void moveRotate(byte angle, bool mode = 0){//expecting input to be from -90 to 90 degrees
//     if(mode){//for obstacle avoidance, used when we go away from the line to pick up a target
//         startSensor_ISR();
//         noInterrupts();
//         readDirection = 0;//read back sensors while pivoting
//         follow_or_obj = 1;
//         interrupts();
//    }
//    int desired_ticks;
//    
//    desired_ticks = angle / 90.0;
////    if(angle > 126){//change input to be able to turn left
////        desired_ticks = angle - 127 /90.0;
////    }else{
////        desired_ticks = (-1*(126 - angle))/90.0;
////    }
//     desired_ticks = desired_ticks * FULL90ROT; //convert angle to the appropriate wheel rotation amount
//     logMove(0,desired_ticks);
//     pivot(desired_ticks);
//}

/** Move the robot forward or reverse
 *
 *	@param ticks number of encoder ticks to move. Sign indicates direction:
 *	positive is forward.
 */

void moveStraight(byte ticks, byte direction = 1, bool mode = 0){//ticks is going to come in as a byte value (0-255)
    int newticks;
    if(direction) newticks = ticks * MOVEMENT_SCALAR;//convert input to the range of ticks we wants
                                          //this value is ~1/5th of a revolution..
                                          //the max distance we can go is ~1056cm with 1 call...
    else newticks = -1 * ticks * MOVEMENT_SCALAR;
    if(mode){//for obstacle avoidance, used when we go away from the line to pick up a target
        startSensor_ISR();
        noInterrupts();
        readDirection = 1;
        follow_or_obj = 1;
        interrupts();
        logMove(1, newticks);
    }
    moveFWBW(newticks);//initiate straight movement routine
}
/*
    Dead reckoning routine to go back to line
*/
int deadReckoning(void){
    while(move_log_size > 0){ //start from end of array and move to beginning
            if(move_log[move_log_size - 1].typeMove){
                moveFWBW(-1 * move_log[move_log_size - 1].ticks);
            }else{
                pivot(-1 * move_log[move_log_size - 1].ticks);
            }
            move_log_size--;
            while(!done_with_move);
    }
    return 1;//complete
}
/*
    function to put a move into the roboMove array,
    usually called before the initiation of a move
*/
int logMove(int type, int ticks){
    if(move_log_size < LOGSIZE){
        roboMove temp = {type, ticks};
        move_log[move_log_size] = temp;
        move_log_size++;
        return 1;
    }else{
        return 0; //log full, either send it to data logger or increase size of log array
    }
}
/**	Routine to follow the guide-line for some fixed interval
 *
 *	This function assumes that we are already over the line
 */
void readLines(){
    //Simple line following routine
    unsigned int left = readLine_left();
    unsigned int right = readLine_right();

    if (left <= 70){ //if line detected left
        moveRotate(ARDUINO_LEFT,30);
        while(!done_with_move);//busywait
    }
  else if (right <= 70){ //if found right
      moveRotate(ARDUINO_RIGHT, 30);
      while(!done_with_move);
    }
  else //if not found go forward a certain amount
    moveStraight(100);
}

void moveLineFollow(void){ //this will actually turn on the line reading in the timer interrupt
    noInterrupts();
    follow_or_obj = 0;
    done_with_command = 0;
    interrupts();
    startSensor_ISR();//
}

void stopLineFollow(void){
    stop_motors();
    stopSensor_ISR();
}

/*
    Avoid obstacle thats in the front of the robot. Do this by turning a certain amount,
    then checking to see if it is still setting off one of the front sensors.
*/
//
// int avoidFrontObstacle(int case){
//
//     // identify what case it is
//     do{
//         switch(case){
//             case -1: //Obstacle is closer to the right sensor
//                 moveRotate(255); //pivot left 30 degrees
//
//                 break;
//             case 1: //Obstacle is closer to the left sensor
//                 moveRotate(0); //pivot right 30 degrees
//
//                 break;
//             case 0: //No obstacle within risk distance sensed...
//         }
//         //check flag again...
//         noInterrupts();
//         int currentSense = sensorFlag;
//         interrupts();
//     }while(currentSense != 0);
//
//     //go forward some amount...
//     moveStraight(40);
//     //rotate back
//     moveRotate(-1*logrotate);
//      //check Camera to require target...
//      //serial communitcation goes here or return some kind of value to send
//
// }

/*
    This function will be called when we are trying to pivot and a side sensor is triggered.
    This will probably depend on our distance that will set off the trigger. But we may want to get
    as close as possible by pivoting then just go straight.
*/
// int avoidSideObstacle(){
//
//     moveStraight(30);
//
// }

/******************************************************************************/
//                                 ARM CONTROLS
/******************************************************************************/

LobotServoController xArm(Serial1);

/**	Move the arm to its parking position
 *
 *	The parking position needs to leave a clear view of the pickup area, but also
 *	should move the center of gravity as far forward as possible to reduce drive
 *	wheel slippage.
 */
void armPark(void){

}

/** Routine to dispose of a syringe once it has been picked.
 *
 */
void armDispose(void){

}

/**	Routine to attempt target pickup
 *
 *	This routine should attempt to close the claw completely and detect if an
 *	object as actually been grabbed. parameters should be bytes because they will
 *	have to be transmitted over serial from the pi. Ranges on these values TBD as
 *	convenient for the arm software, but must be recorded in the system constants
 *	file.
 *	--ABD
 *
 *	@param azimuth arm azimuth value
 *	@param range distance to the target
 *	@param orientation rotation of the target
 *
 *	@return true on successful pick, false otherwise.
 */


 void grabObject(byte angle, byte radius, byte handAngle){

  LobotServo servos[6];
  //Calculate position based off the angle for ID:6
  //756 is a constant position for the ratio of 180 degrees to position number
  servos[5].ID = 6;
  servos[5].Position = (angle/180.0)*760 + 100;
  Serial.println(servos[5].Position);
  servos[1].ID = 2;
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
  delay(3000);
  //Drop the needle
  //Run action group 1, 1 time
  xArm.runActionGroup(1, 1);
  delay(4500);
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
