##	@package Syringenator The top-level Pi program.
#	@file Syringenator.py
#	This is the main control script. It will run on the Raspberry Pi and direct
#	all robot operations.
#
#	By convention each arduino command routine checks for arduino ready before
#	starting, and logs arduino status on exit.
#
#	@todo how do we initialize the robot run? a button press?
#	--ABD
#
#	@copyright Copyright &copy; 2019 by the authors. All rights reserved.


#==============================================================================#
#                                LIBRARIES
#==============================================================================#


import constants
import cv2
import serial


#==============================================================================#
#                               DATA LOGGING
#==============================================================================#

## Record system events for later analysis
#
# @returns None
def log(arg):
	if arg is string:
		pass # log the string
	if arg is Target:
		pass # log target aquired


#==============================================================================#
#                             ARDUINO COMMUNICATION
#==============================================================================#


##	Send serial data to the arduino
#
#	@param bytes one or more bytes of data to send to the arduino
#	@returns None
def arduinoSend(bytes):
	pass

##	Wait some fixed time for the arduino to send one or more bytes
#
#	@returns a list of bytes
def arduinoReceive():
	return None


#==============================================================================#
#                           GEOMETRIC TRANSFORMATIONS
#==============================================================================#


##	@page calibration Calibration
#
#	# Coordinate Systems
#	This robot, of necessity uses multiple sets of coordinates.
#
#	## Image Cartesian
#	This coordinate system is used to locate pixels and distance measurements in
#	the images generated from the camera. It consists of a positive integer tuple
#	horizontal and vertical. Its axes are at right angles, and its origin is in
#	the upper left corner of the image. Its values are always positive and its
#	units are pixels.
#
#	We may also consider the camera's depth value as the third member of the
#	image coordinates. Its units should be meters.
#
#	## Floor Cartesian
#	This coordinate system is used to locate targets around the robot. It
#	consists of a signed integer tuple fore-aft and port-starboard. Positive
#	values are forward and starboard. Its axes are at right angles and its origin
#	is directly below the origin of Image Cartesian.
#	Its units of length are centimeters. Smaller units introduce unecessary and
#	likely unrealistic precision. Larger units would require this system to use
#	floats.
#
#	## Arm Cylindrical
#	This coordinate system is used to locate targets around the xArm. It consists
#	of an unsigned integer tuple azimuth and range. Its origin is at the level of
#	the floor and directly below the xArm axis of rotation. Its units are those
#	convenient for the use of the arm, and its range of values is recorded in
#	constants.in

##	Derive floor position from image data
#
#	@param x the x-value of the point of interest in the image
#	@param y the y-value of the point of interest in the image
#	@param d the distance value of the point of interest in the image
#	@returns a tuple (x, y)
def imageCart2floorCart(x, y, d):
	pass

##	Derive cylindrical coordinates, centered on the arm from cartesian
#	coordinates centered on the camera.
#	
#	@param x the x-value of the point of interest on the floor
#	@param y the y-value of the point of interest on the floor
#	@returns a tuple (Azimuth, Range)
def floorCart2armCylinder(x, y):
	pass


#==============================================================================#
#                              IMAGE COMMANDS
#==============================================================================#


## A class to contain everything we know about an aquired target
class Target: pass
	# bounding box data
	# bounding box center
	# raw image data

##	A routine to take a picture and report back the closest target
#	The Computer vision routine must be able to handle multiple targets in the
#	image. It would be best if all targets are reported. Then this routine will
#	determine the closest one to pursue.
#	--ABD
#
#	@returns a target object
def scan():
	# get a picture from librealsense
	
	# pass the picture to OpenCV
	
	
	# if there is a target construct the target object
	target = Target()
	
	# calculate x=(x_1+x_2)/2 and y=(y_1+y_2)/2
	
	return None


##	A routine to determine if the target is in position to be picked up.
#
#	Calculates whether the center of the target bounding box is in the pickup area.
#
#	@returns a boolean
def canBePicked(t):
	
	
	# is the target within the pick area?
	if t.x > PICKUP_X_MIN and t.x < PICKUP_X_MAX and y > PICKUP_Y_MIN and y < PICKUP_Y_MAX:
		return True
	else:
		return False


#==============================================================================#
#                                ARDUINO COMMANDS
#==============================================================================#


##	Move the robot closer to the given target.
#	The moveCloser() routine attempts to aproach the target by relatively small
#	increments. Because the move routines may be interrupted by the obstacle
#	avoidance ISRs and the risk of jambing the wheels etc. we cannot expect to be
#	able to approch successfully on the first try. Hence moveCloser() should only
#	move a relatively short distance before exiting to allow another loop through
#	the scan cycle.
#
#	Should we spend effort trying to avoid running over decoys here?
#
#	This routine should check for ARDUINO_STATUS_OBSTACLE. then what?
#
#	This routine is likely where we will have the most issues.
#	--ABD
#
#	@param t a Target object containing the location of the target to be approched
#	@returns None
def moveCloser(t):
	log("moveCloser(): start")
	while(arduinoReceive() != ARDUINO_STATUS_READY):
		pass
	
	# face the target if necessary
	if t.x < PICKUP_X_MIN:
		rotTicks = CAL_ROT_FACTOR*(PICKUP_X_MIN-t.x) # positive rotation
		if rotTicks > ROT_MAX_TICKS:
			rotTicks = ROT_MAX_TICKS
		
		log("ARDUINO_ROTATE: ", rotTicks)
		arduinoSend(ARDUINO_ROTATE, rotTicks)
	
	elif t.x> PICKUP_X_MAX:
		rotTicks = CAL_ROT_FACTOR*(PICKUP_X_MAX-t.x) # negative rotation
		if rotTicks < -ROT_MAX_TICKS:
			rotTicks = -ROT_MAX_TICKS
		
		log("ARDUINO_ROTATE: ", rotTicks)
		arduinoSend(ARDUINO_ROTATE, rotTicks)
	
	# move forward if necessary
	# the pixel origin is in the upper left corner
	if t.y < PICKUP_Y_MIN:
		fwdTicks = CAL_FWD_FACTOR*(PICKUP_Y_MIN-t.y) # positive translation
		if fwdTicks > FWD_MAX_TICKS:
			fwdTicks = FWD_MAX_TICKS
		
		log("ARDUINO_MOVE: ", fwdTicks)
		arduinoSend(ARDUINO_MOVE, fwdTicks)
	
	elif t.y > PICKUP_Y_MAX: # this may not work as expected
		fwdTicks = CAL_FWD_FACTOR*(PICKUP_Y_MAX-t.y) # negative translation
		if fwdTicks < -FWD_MAX_TICKS:
			fwdTicks = -FWD_MAX_TICKS
		
		log("ARDUINO_MOVE: ", fwdTicks)
		arduinoSend(ARDUINO_MOVE, fwdTicks)
	
	# in all cases we wait for the arduino to be ready
	while((status=arduinoReceive()) == None):
		pass
	log("moveCloser(): status is", status)
	if status == ARDUINO_STATUS_OBSTACLE:
		obstacle = True


## avoid an obstacle
#
#	@returns None
def avoid():
	log("avoid(): start")
	while(arduinoReceive() != ARDUINO_STATUS_READY):
		pass
	
	arduinoSend(ARDUINO_AVOID)
	
	# in all cases we wait for the arduino to be ready
	while((status=arduinoReceive()) == None):
		pass
	log("avoid(): status is", status)
	obstacle = False



##	Attempt to pickup and dispose the target.
#	This routine must determine orientation of the target. If this is not done by
#	some OpenCV magic we can attempt it here using the raw image data and the
#	bounding box.
#
#	Divide the longer dimension of the bounding box by some constant divisor. Scan
#	along each of those raster lines twice. On the first pass calculate an average
#	brightness (RGB values can be summed). The second pass will pick out points of
#	greatest brightness. Find the centers of clustered bright pixeles. We now have
#	a set of points in cartesian space. Have Jake find the slope of the line of
#	best fit.
#
#	The center can be estimated as the center of the bounding box, or the center of
#	the points, the mean of both, etc.
#
#	Once the values for x, y, and m have been determined they will have to pass
#	through a calibration transform to determine the arm a, r, o values.
#	--ABD
#
#	@param t a Target object containing the raw bitmap data
#	@returns None
def pickUp(t):
	log("pickUp(): start")
	while(arduinoReceive() != ARDUINO_STATUS_READY):
		pass
	
	# find the center and orientation of the target
	
	# signal the arduino to pickUp
	
	# in all cases we wait for the arduino to be ready
	while((status=arduinoReceive()) == None):
		pass
	log("pickUp(): status is", status)


##	signl the arduino to return to the line.
#
#	@todo do we need to check that we actually returned? how do we recover if
#	dead reckoning fails? --ABD
#
#	We disscussed the possibility of a timer on lineFollow(), that if the line
#	has not been detected recently then we know we are off track and must recoves
#	somehow.
#
#	@returns None
def returnToLine():
	log("returnToLine(): start")
	while(arduinoReceive() != ARDUINO_STATUS_READY):
		pass
	
	# in all cases we wait for the arduino to be ready
	while((status=arduinoReceive()) == None):
		pass
	log("returnToLine(): status is", status)

##	Follow the line.
#
#	this routine simply signals the arduino to execute its lineFollow() routine
#
#	@returns None
def lineFollow():
	log("lineFollow(): start")
	while(arduinoReceive() != ARDUINO_STATUS_READY):
		pass
	
	# in all cases we wait for the arduino to be ready
	while((status=arduinoReceive()) == None):
		pass
	log("lineFollow(): status is", status)


#==============================================================================#
#                                   MAIN LOOP
#==============================================================================#


## boolean indicating whether we are on the line
onTheLine = True
## boolean indicating that we have detected an obstacle
obstacle = False
## The currently aquired target
target = None

while True:
	target = scan()
	if target != None: # we have aquired a target
		log(target)
		if canBePicked(target):
			pickUp(target)
		elif obstacle:
			# we can't pick the target, and moveCloser() failed with an obstacle
			avoid()
		else:
			onTheLine = False
			moveCloser(target)
	elif onTheLine:
		lineFollow()
	else:
		returnToLine()
		onTheLine = True




