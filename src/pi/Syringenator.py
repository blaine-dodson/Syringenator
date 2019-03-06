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


DEBUG_CAPTURE = False
DEBUG_AQUISITION = True
DEBUG_APPROACH = True
DEBUG_TRANSFORM = True
DEBUG_ORIENTATION = True
DEBUG_TIMING = True
DEBUG_CAL = False

import constants
import cv2
import numpy
import pyrealsense2
import coordinateXforms as xform
import SySerial


if DEBUG_TIMING: import time

if DEBUG_CAPTURE or DEBUG_AQUISITION or DEBUG_APPROACH or DEBUG_CAL:
	cv2.namedWindow("View", cv2.WINDOW_AUTOSIZE );


#==============================================================================#
#                                 DEFINITIONS
#==============================================================================#


## A class to contain everything we know about an aquired target
class Target:
	# bounding box data
	# bounding box center
	# raw image data
	def __init__(self, box, score, center):
		self.confidence  = score
		self.centerX = center[0]
		self.centerY = center[1]
		self.box = box
	
	def setImg(self, img):
		self.image = img
	
	##	Get the taxicab distance to the target.
	#	@returns an integer representing distance
	def distance(self):
		xDist = self.centerX - (IMG_WIDTH/2)
		yDist = IMG_HEIGHT - self.centerY
		
		if(xDist<0): xDist = -xDist
		
		return xDist + yDist
	
	def getBox(self):
		return self.box



class NeuralNet:
	NETREZ = 320
	WEIGHTSPATH = "nn/yolov3-tiny-obj_37000.weights"
	CONFIGPATH = "nn/yolov3-tiny-obj.cfg"
	
	def __init__(self):
		self.nn = cv2.dnn.readNetFromDarknet(self.CONFIGPATH, self.WEIGHTSPATH)
		layers = self.nn.getLayerNames()
		self.ln = [layers[i[0] - 1] for i in self.nn.getUnconnectedOutLayers()]
		log("string", "NeuralNet(): net loaded")
	
	def detect(self, img):
		blob = cv2.dnn.blobFromImage(
			img, 1 / 255.0, (self.NETREZ,self.NETREZ), swapRB=True
		)
		self.nn.setInput(blob)
		if(DEBUG_TIMING): start = time.time()
		output = self.nn.forward(self.ln)
		if(DEBUG_TIMING): end = time.time()

		# show timing information on YOLO
		if(DEBUG_TIMING):
			log("string", "YOLO took {:.6f} seconds".format(end - start))
		return output



class Camera:
	def __init__(self):
		cfg = pyrealsense2.config()
	
		#enable_stream(stream_type: rs.stream, width: int, height: int, format: rs.format=format.any, framerate: int=0L) -> None
		cfg.enable_stream(
			pyrealsense2.stream.color,
			IMG_WIDTH, IMG_HEIGHT, 
			pyrealsense2.format.bgr8,
			FRAME_RATE
		)
	
		self.pipeline = pyrealsense2.pipeline()
	
		if(self.pipeline == None):
			og('string', "Pipeline not created")
			exit() # @todo fix this

		try:
			# Create a context object. This object owns the handles to all connected
			# realsense devices
			self.pipeline.start(cfg)
		except:
			log('string', "Pipeline did not start")
			exit() # @todo fix this

		# stabilize auto exposure. do we need to do this once, or before each pic?
		for i in range(0,30):
			frames = self.pipeline.wait_for_frames()
	
		log('string', "Camera initialized")
	
	def capture(self):
		try:
			frames = self.pipeline.wait_for_frames()
		except:
			print("no frames")
			initCamera()
			return None
	
		frame = frames.get_color_frame()
	
		# how often does this happen?
		if(frame == None):
			print("no frame")
			return None
	
		mat = numpy.reshape(
			numpy.array(frame.get_data(), numpy.uint8),
			(IMG_HEIGHT, IMG_WIDTH, 3)
		)
	
		if(DEBUG_CAPTURE):
			#cv2.namedWindow("Image Capture", cv2.WINDOW_AUTOSIZE );
			log("string", "photo capture")
			cv2.imshow("View", mat);
			cv2.waitKey(500);
	
		return mat


#==============================================================================#
#                                 GLOBALS
#==============================================================================#


# Realsense Parameters
FRAME_RATE = 30
IMG_WIDTH  = 640
IMG_HEIGHT = 480
FOV_X      = 63.4
FOV_Y      = 40.4

# OpenCV Parameters
CONFIDENCE   =.5
NMS_THRESHOLD=.1


#==============================================================================#
#                               DATA LOGGING
#==============================================================================#


## Record system events for later analysis
#
# @returns None
def log(datatype, *args):
	if(datatype == 'string'): # log the string
		for x in args: print(x)
	elif(datatype == 'target'):
		print("Target aquired")
		print(
				"x: "+str(args[0].centerX)+" y: "+str(args[0].centerY)
			)
	else:
		print("log(): unknown type")


#==============================================================================#
#                        EXTRACT TARGETS FROM CV OUTPUT
#==============================================================================#


# scale the bounding box coordinates back relative to the
# size of the image, keeping in mind that YOLO actually
# returns the center (x, y)-coordinates of the bounding
# box followed by the boxes' width and height
def rescale(detection):
	box = detection[0:4] * numpy.array(
		[IMG_WIDTH, IMG_HEIGHT, IMG_WIDTH, IMG_HEIGHT])
	(centerX, centerY, width, height) = box.astype("int")
	return (centerX, centerY, width, height)


def extractTargets(dataIn):
	boxes = []
	centers = []
	confidences = []
	targets = []
	
	log("string", "extractTargets(): start")
	
	# loop over each of the layer outputs
	for output in dataIn:
		# loop over each of the detections
		for detection in output:
			# extract the class ID and confidence (i.e., probability) of
			# the current object detection
			scores = detection[5:]
			classID = numpy.argmax(scores)
			confidence = scores[classID]

			# filter out weak predictions by ensuring the detected
			# probability is greater than the minimum probability
			if confidence > CONFIDENCE:
				(centerX, centerY, width, height) = rescale(detection)

				# use the center (x, y)-coordinates to derive the top
				# left corner of the bounding box
				x = int(centerX - (width / 2))
				y = int(centerY - (height / 2))

				# update our list of bounding box coordinates, confidences,
				# and class IDs
				boxes.append([x, y, int(width), int(height)])
				confidences.append(float(confidence))
				centers.append([centerX, centerY])


	# apply non-maxima suppression to suppress weak, overlapping bounding
	# boxes
	idxs = cv2.dnn.NMSBoxes(boxes, confidences, CONFIDENCE,
		NMS_THRESHOLD)
	
	if len(idxs) > 0:
		for i in idxs.flatten():
			targets.append(Target(boxes[i], confidences[i], centers[i]))
	
	
	log("string", "extractTargets(): finish")
	return targets


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
#	is at the front edge of the robot.
#	Its units of length are millimeters.
#
#	## Arm Cylindrical
#	This coordinate system is used to locate targets around the xArm. It consists
#	of an unsigned integer tuple azimuth and range. Its origin is at the level of
#	the floor and directly below the xArm axis of rotation. Its units are those
#	convenient for the use of the arm, and its range of values is recorded in
#	constants.in



#XPIX2LEN = (numpy.tan(FOV_X/2*numpy.pi/180))/(IMG_WIDTH/2)*constants.CAL_CAM_AXIS
#YPIX2LEN = (numpy.tan(FOV_Y/2*numpy.pi/180))/(IMG_HEIGHT/2)*constants.CAL_CAM_AXIS

XPIX2LEN = 2
YPIX2LEN = 10/7.0

##	Derive floor position from image data
#
#	@param x the x-value of the point of interest in the image
#	@param y the y-value of the point of interest in the image
#	@param d the distance value of the point of interest in the image
#	@returns a tuple (x, y) the coordinates on the floor in mm
def imageCart2floorCart(t):
	
#	if DEBUG_TRANSFORM:
#		print "XPIX2LEN: " + str(XPIX2LEN)
#		print "YPIX2LEN: " + str(YPIX2LEN)
	
	xf = int(XPIX2LEN*(t.centerX - IMG_WIDTH/2))
	yf = int(YPIX2LEN*(IMG_HEIGHT - t.centerY))
	
	if DEBUG_TRANSFORM:
		print "xf: " + str(xf)
		print "yf: " + str(yf)
	
	return (xf, yf)

##	Derive cylindrical coordinates, centered on the arm from cartesian
#	coordinates centered on the camera.
#	
#	@param x the x-value of the point of interest on the floor
#	@param y the y-value of the point of interest on the floor
#	@returns a tuple (Azimuth, Range)
def floorCart2armCylinder((x, y)):
	y += constants.CAL_ARM_OFFSET
	
	#r = numpy.rint(numpy.sqrt(x**2 + y**2)/10)
	r  = xform.fndRadius(y,x)
	
	if x>0:
		az = int(numpy.rint(180-(180/numpy.pi) * numpy.arctan(y/x)))
	elif x<0:
		az = int(numpy.rint(-(180/numpy.pi) * numpy.arctan(y/x)))
	else:
		az = 90
	
	return (az, r)


#def armCoordinates(t):
#	
#	x = xform.estCoordinateX(
#		constants.CAL_CAM_HEIGHT,
#		IMG_WIDTH/2, IMG_HEIGHT/2,
#		constants.CAL_CAM_ANGLE,
#		FOV_X, FOV_Y,
#		t.centerX, t.centerY,
#		constants.CAL_ARM_OFFSET
#	)
#	
#	z = xform.estCoordinateZ(
#		constants.CAL_CAM_HEIGHT,
#		IMG_HEIGHT/2,
#		constants.CAL_CAM_ANGLE,
#		FOV_Y,
#		t.centerY,
#		constants.CAL_ARM_OFFSET
#	)
#	
#	if DEBUG_TRANSFORM:
#		print("x value: " + str(x))
#		print("z value: " + str(z))
#	
#	theta = xform.findTheta(z,x)
#	
#	radius = xform.fndRadius(z,x)
#	
#	phi = xform.orientationCapture(
#		int(t.box[0]), int(t.box[1]), int(t.box[2]), int(t.box[3]), t.image)
#	
#	if DEBUG_TRANSFORM:
#		log("string", "Theta : " + str(theta))
#		log("string", "Radius: " + str(radius))
#		log("string", "Phi   : " + str(phi))
#	
#	return (theta, radius, phi)



#==============================================================================#
#                              IMAGE COMMANDS
#==============================================================================#


##	A routine to take a picture and report back the closest target
#	The Computer vision routine must be able to handle multiple targets in the
#	image. It would be best if all targets are reported. Then this routine will
#	determine the closest one to pursue.
#	--ABD
#
#	@param pipe a realsense2 pipeline object configured with a color stream.
#	@returns a target object
def scan(cam, net):
	log("string", "scan(): start")
	
	# get a picture from librealsense
	image = cam.capture()
	
	# pass the picture to OpenCV
	cvOutput = net.detect(image)
	
	targets = extractTargets(cvOutput)
	
	if(DEBUG_AQUISITION):
		for t in targets:
			(x, y, w, h) = t.box
			
			text = "{}: {:.4f}".format("syringe", t.confidence)
			
			print(
				"x: "+str(x)+" y: "+str(y)+" w: "+str(w)+" h: "+str(h)+" "+text
			)
			
			# draw a bounding box rectangle and label on the image
			cv2.rectangle(image, (x, y), (x + w, y + h), [102, 220, 225], 2)
			
			#can get rid of
			cv2.putText(
				image, text, (x, y - 5), cv2.FONT_HERSHEY_SIMPLEX,
				0.5, [102, 220, 225], 2
			)
		
		
		cv2.imshow("View", image)
		cv2.waitKey(1000)
	
	# pick the closest target
	d = 2000000
	closest = None
	
	for t in targets:
		#print("target distance is: " + str(t.distance()))
		#print("d is: " + str(d))
		if(t.distance() < d):
			closest = t
			d = closest.distance()
			#print("updating closest")
	
	if(closest != None):
		closest.setImg(image)
	
	log("string", "scan(): stop")
	return closest

##	A routine to determine if the target is in position to be picked up.
#
#	Calculates whether the center of the target bounding box is in the pickup area.
#
#	@returns a boolean
def canBePicked(t):
	# is the target within the pick area?
	if(
		t.centerX > constants.PICKUP_X_MIN and
		t.centerX < constants.PICKUP_X_MAX and
		t.centerY > constants.PICKUP_Y_MIN and
		t.centerY < constants.PICKUP_Y_MAX
	):
		log("string", "can pick")
		if DEBUG_APPROACH:
			cv2.rectangle(
				t.image,
				(constants.PICKUP_X_MIN, constants.PICKUP_Y_MIN),
				(constants.PICKUP_X_MAX, constants.PICKUP_Y_MAX),
				[200, 0, 0], 2
			)
			cv2.drawMarker(
				t.image,
				(t.centerX, t.centerY),
				[0, 200, 0]
			)
			# show the output image
			#cv2.namedWindow("Approach", cv2.WINDOW_AUTOSIZE );
			cv2.imshow("View", t.image)
			cv2.waitKey(1000)
		return True
	else:
		log("string", "cannot pick")
		if DEBUG_APPROACH:
			cv2.rectangle(
				t.image,
				(constants.PICKUP_X_MIN, constants.PICKUP_Y_MIN),
				(constants.PICKUP_X_MAX, constants.PICKUP_Y_MAX),
				[220, 0, 0], 2
			)
			cv2.drawMarker(
				t.image,
				(t.centerX, t.centerY),
				[0, 0, 150]
			)
			# show the output image
			#cv2.namedWindow("Approach", cv2.WINDOW_AUTOSIZE );
			cv2.imshow("View", t.image)
			cv2.waitKey(1000)
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
#	This routine is likely where we will have the most issues.
#	--ABD
#
#	@param t a Target object containing the location of the target to be approched
#	@returns None
def approach(t):
	log("string", "approach(): start")
	while(comPort.status() != constants.ARDUINO_STATUS_READY):
		pass
	
	# face the target if necessary
	if t.centerX < constants.PICKUP_X_MIN: # positive rotation
		rotTicks = constants.CAL_ROT_FACTOR*(constants.PICKUP_X_MIN-t.centerX) 
		if rotTicks > constants.ROT_MAX_TICKS:
			rotTicks = constants.ROT_MAX_TICKS
		
		log("string", "ARDUINO_ROTATE: " + str(rotTicks))
		comPort.send([constants.ARDUINO_ROTATE, rotTicks])
	
	elif t.centerX > constants.PICKUP_X_MAX: # negative rotation
		rotTicks = constants.CAL_ROT_FACTOR*(constants.PICKUP_X_MAX-t.centerX) 
		if rotTicks < -constants.ROT_MAX_TICKS:
			rotTicks = -constants.ROT_MAX_TICKS
		
		log("string", "ARDUINO_ROTATE: " + str(rotTicks))
		comPort.send([constants.ARDUINO_ROTATE, rotTicks])
	
	# in all cases we wait for the arduino to be ready
	status = None
	while(status == None):
		status = comPort.status()
	log("string", "approach(): status is " + str(status) )
	
	# move forward if necessary
	# the pixel origin is in the upper left corner
	if t.centerY < constants.PICKUP_Y_MIN: # positive translation
		fwdTicks = constants.CAL_FWD_FACTOR*(constants.PICKUP_Y_MIN-t.centerY)
		if fwdTicks > constants.FWD_MAX_TICKS:
			fwdTicks = constants.FWD_MAX_TICKS
		
		log("string", "ARDUINO_MOVE: " + str(fwdTicks))
		comPort.send([constants.ARDUINO_MOVE, fwdTicks])
	
	elif t.centerY > constants.PICKUP_Y_MAX: # negative translation
		# this may not work as expected
		fwdTicks = constants.CAL_FWD_FACTOR*(constants.PICKUP_Y_MAX-t.centerY)
		if fwdTicks < -constants.FWD_MAX_TICKS:
			fwdTicks = -constants.FWD_MAX_TICKS
		
		log("string", "ARDUINO_MOVE: " + str(fwdTicks) )
		comPort.send([constants.ARDUINO_MOVE, fwdTicks])
	
	# in all cases we wait for the arduino to be ready
	status = None
	while(status == None):
		status = comPort.status()
	log("string", "approach(): status is " + str(status) )
	if status == constants.ARDUINO_STATUS_OBSTACLE:
		obstacle = True
		log("string", "obstacle detected")


## avoid an obstacle
#
#	@returns None
def avoid():
	log("avoid(): start")
	while(comPort.status() != constants.ARDUINO_STATUS_READY):
		pass
	
	comPort.send([constants.ARDUINO_AVOID])
	
	# in all cases we wait for the arduino to be ready
	status = None
	while(status == None):
		status = comPort.status()
	log("string", "avoid(): status is " + str(status) )
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
	log("string", "pickUp(): start")
	while(comPort.status() != constants.ARDUINO_STATUS_READY):
		pass
	
	# find the center and orientation of the target
	(theta,r) = floorCart2armCylinder(imageCart2floorCart(t))
	
	phi = xform.orientationCapture(
		int(t.box[0]), int(t.box[1]), int(t.box[2]), int(t.box[3]), t.image)
	
	# limit the ranges
	if theta>constants.ARM_AZIMUTH_MAX: theta = constants.ARM_AZIMUTH_MAX
	if theta<constants.ARM_AZIMUTH_MIN: theta = constants.ARM_AZIMUTH_MIN
	
	if r>constants.ARM_RANGE_MAX: r = constants.ARM_RANGE_MAX
	if r<constants.ARM_RANGE_MIN: r = constants.ARM_RANGE_MIN
	
	if phi>constants.ARM_ORIENT_MAX: phi = constants.ARM_ORIENT_MAX
	if phi<constants.ARM_ORIENT_MIN: phi = constants.ARM_ORIENT_MIN
	
	if DEBUG_TRANSFORM:
		print "theta: " + str(theta) + " type: " + str(type(theta))
		print "r: " + str(r) + " type: " + str(type(r))
		print "phi: " + str(phi) + " type: " + str(type(phi))
	
	# signal the arduino to pickUp
	comPort.send([constants.ARDUINO_ARM_PICKUP, theta, r, phi])
	
	# in all cases we wait for the arduino to be ready
	status = None
	while(status == None):
		status = comPort.status()
	log("string", "pickUp(): status is " + str(status) )


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
	log("string", "returnToLine(): start")
	while(comPort.status() != constants.ARDUINO_STATUS_READY):
		pass
	
	comPort.send([constants.ARDUINO_RETURN])
	
	# in all cases we wait for the arduino to be ready
	status = None
	while(status == None):
		status = comPort.status()
	log("string", "returnToLine(): status is " + str(status) )

##	Follow the line.
#
#	this routine simply signals the arduino to execute its lineFollow() routine
#
#	@returns None
def lineFollow():
	log("string", "lineFollow(): start")
	while(comPort.status() != constants.ARDUINO_STATUS_READY):
		pass
	
	comPort.send([constants.ARDUINO_LINE_FOLLOW])
	
	# in all cases we wait for the arduino to be ready
	status = None
	while(status == None):
		status = comPort.status()
	log("string", "lineFollow(): status is " + str(status) )


#==============================================================================#
#                               INITIALIZATION
#==============================================================================#


## boolean indicating whether we are on the line
onTheLine = True
## boolean indicating that we have detected an obstacle
obstacle = False
## The currently aquired target
target = None

camera = Camera()
neuralNet = NeuralNet()
comPort = SySerial.ComPort()


#==============================================================================#
#                                    MAIN LOOP
#==============================================================================#


while DEBUG_CAL:
	image = camera.capture()
	cv2.line(image, (IMG_WIDTH/2,0), (IMG_WIDTH/2,IMG_HEIGHT), [255,0,255])
	
	for y in range(constants.PICKUP_Y_MAX, constants.PICKUP_Y_MIN, -10):
		cv2.line(image, (0,y), (IMG_WIDTH,y), [255,0,0])
	
	
	cv2.rectangle(
		image,
		(constants.PICKUP_X_MIN, constants.PICKUP_Y_MIN),
		(constants.PICKUP_X_MAX, constants.PICKUP_Y_MAX),
		[200, 0, 0], 2
	)
	cv2.imshow("View", image)
	crop = image[
		constants.PICKUP_Y_MIN:constants.PICKUP_Y_MAX,
		constants.PICKUP_X_MIN:constants.PICKUP_X_MAX
	]
	
	
	bigger = cv2.resize(src=crop, dsize=(0,0), fx=4, fy=4)
	cv2.imshow("Bigger", bigger)
	cv2.waitKey(0)


log('string', "Syringenator: Start")
while True:
	target = scan(camera,neuralNet)
	if target != None: # we have aquired a target
		log("target", target)
		if canBePicked(target):
			pickUp(target)
		elif obstacle:
			# we can't pick the target, and moveCloser() failed with an obstacle
			avoid()
		else:
			onTheLine = False
			approach(target)
	elif onTheLine:
		lineFollow()
	else:
		returnToLine()
		onTheLine = True




