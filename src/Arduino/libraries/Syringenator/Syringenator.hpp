/**	@file Syringenator.hpp
 *
 *	Arduino controller code
 *	--ABD
 *
 *	@copyright Copyright &copy; 2019 by the authors. All rights reserved.
 */

#include "constants.h"
#include "MotorLib.h"
#include "SensorLib.h"
#include "Arduino.h"
#include <LobotServoController.h>

//A simple struct for storing locomotion movements
struct roboMove{
    bool typeMove; //true == moveStraight
                   //false == pivot
    int ticks;
};

#define LOGSIZE 256 //for the deadReckoning logger
#define FULL90ROT_RIGHT  875//the number of ticks it takes to rotate the robot 90 degrees
#define FULL90ROT_LEFT   835

extern volatile bool done_with_command; //for deadReckoning and other routines that have subflags

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

int setupSensor_ISR();
ISR(TIMER3_COMPA_vect);
void stopSensor_ISR();
void startSensor_ISR();

/**	A function to handle incomming communication from the pi.
 *
 */

void serialCommunication_ISR(void);
int isDoneCommand(int type_command);//check for done flag of command type
bool isReady(void);


/******************************************************************************/
//                              LOCAMOTION CONTROL
/******************************************************************************/

/**	Rotate the robot around central axis
 *	rotate by running both motors at the same speed in opposite directions
 *
 *	@param ticks sign indicates direction of rotation: positive is rotation to
 *	the right. magnitude indicates the number of encoder ticks on each motor.
 */

void moveRotate(byte direction, byte angle, bool mode = 0);

/** Move the robot forward or reverse
 *
 *	@param ticks number of encoder ticks to move. Sign indicates direction:
 *	positive is forward.
 *  @param mode: 0 = line follow, 1 = object avoidance
 */

void moveStraight(byte ticks, byte direction = 1, bool mode = 0);
/*
    Dead reckoning routine to go back to line
*/
int deadReckoning(void);

/**	Routine to follow the guide-line for some fixed interval
 *
 *	This function assumes that we are already over the line
*/

int logMove(int type, int ticks);

void stopLineFollow(void);
void moveLineFollow(void);
void readLines();

/*
    Avoid obstacle thats in the front of the robot. Do this by turning a certain amount,
    then checking to see if it is still setting off one of the front sensors.
*/
// int avoidFrontObstacle(int case);
// /*
//     This function will be called when we are trying to pivot and a side sensor is triggered.
//     This will probably depend on our distance that will set off the trigger. But we may want to get
//     as close as possible by pivoting then just go straight.
// */
// int avoidSideObstacle();
/******************************************************************************/
//                                 ARM CONTROLS
/******************************************************************************/


/**	Move the arm to its parking position
 *
 *	The parking position needs to leave a clear view of the pickup area, but also
 *	should move the center of gravity as far forward as possible to reduce drive
 *	wheel slippage.
 */
void armPark(void);

/** Routine to dispose of a syringe once it has been picked.
 *
 */
void armDispose(void);

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
 void grabObject(byte angle, byte radius, byte handAngle);
