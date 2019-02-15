##	@package Syringenator The top-level Pi program.
#	@file Syringenator.py
#	This is the main control script. It will run on the Raspberry Pi and direct
#	all robot operations.
#	@todo how do we initialize the robot run? a button press?
#	--ABD
#
#	@copyright Copyright &copy; 2019 by the authors. All rights reserved.


import constants


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
	pass


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
#                                MAIN ROUTINES
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
	
	# face the target if necessary
	
	pass

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
	# find the center and orientation of the target
	
	# signal the arduino to pickUp
	
	pass

##	signl the arduino to return to the line.
#
#	@todo do we need to check that we actually returned? how do we recover if
#	dead reckoning fails? --ABD
#
#	@returns None
def returnToLine():
	pass
	
	

##	Follow the line.
#
#	this routine simply signals the arduino to execute its lineFollow() routine
#
#	@returns None
def lineFollow(): pass

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
#                                    MAIN LOOP
#==============================================================================#


## boolean indicating whether we are on the line
onTheLine = True
## The currently aquired target
target = None

while True:
	target = scan()
	if target != None: # we have aquired a target
		log(target)
		if canBePicked(target):
			pickUp(target)
		else:
			onTheLine = False
			moveCloser(target)
	elif onTheLine:
		lineFollow()
	else:
		returnToLine()
		onTheLine = True




