/**	@file Syringenator.hpp 
 *	
 *	Arduino controller code
 *	
 *	--ABD
 */

#include "constants.py"


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
void lineDetector_ISR(void){
	// stop motors if necessary
	
	// contiously poll the line sensor 
	// if the line is still detected, advance the opposite motor.
	// a delay may be appropriate in this loop to allow the motor to advance
	
	
	// clear the interrupt if necessary
	
	// return now that the sensor is clear
}


/**	A function to respond to a detected obstacle while under locamotion.
 *	There may be two cases to handle: whether we are line following, or aproaching.
 *	If we are line following we need to ensure that we don't lose the line while
 *	avoiding the obstacle.
 *	
 *	This may need to be multiple routines, one for each sensor
 *	--ABD
 */
void obstacleDetector_ISR(void){
	
}

/**	Motor encoder ISR
 *	
 */
void motorEncoder_ISR(void){
	
}

/**	A function to handle incomming communication from the pi.
 *	
 */
void serialCommunication_ISR(void){

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
void moveRotate(int ticks){
	
}

/** Move the robot forward or reverse
 *
 *	@param ticks number of encoder ticks to move. Sign indicates direction:
 *	positive is forward.
 */
void moveStraight(int ticks){
	
}

/**	Routine to follow the guide-line for some fixed interval
 *
 *	This function assumes that we are already over the line
 */
void moveLineFollow(void){
	
	// Enable line detector ISR
	
	
	// Disable line detector ISR
	
}


/******************************************************************************/
//                                 ARM CONTROLS
/******************************************************************************/


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
bool armPick(byte azimuth, byte range, byte orientation){
	
}






