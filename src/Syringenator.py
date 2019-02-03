##	@package Syringenator.py
#
#	An outline python script for robot control
#
#	--ABD

import constants


class Target: pass
	# bounding box data


################################################################################
#                               HELPER FUNCTIONS
################################################################################



##	A routine to take a picture and report back the closest target
#	@return a target object
def scan():
	# get a picture from librealsense
	
	# pass the picture to OpenCV
	
	# construct the target object
	target = Target()
	
	return None


def moveCloser(t): pass

def pickUp(t): pass

def returnToLine(): pass

##	Follow the line.
#
#	this routine simply signals the arduino to execute its lineFollow() routine
#
#	@return None
def lineFollow(): pass

##	A routine to determine if the target is in position to be picked up.
#
#	Calculates whether the center of the target bounding box is in the pickup area.
#	x=(x_1+x_2)/2 and y=(y_1+y_2)/2
#
#	@return a boolean
def canBePicked(t):
	pass


#==============================================================================#
#                           TOP LEVEL FLOW CHART
#==============================================================================#

## boolean indicating whether we are on the line
onTheLine = True

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




