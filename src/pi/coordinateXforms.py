import numpy
import cv2

# Returns the estimated y coordinate with respect to the camera. If you 
# want some other y value you need to calculate that offset. 
# @param pi / 2 - angle the camera makes between the mast: beta = 1.2117
# @param the y pixel resolution of the image we're taking: hres_y = 480 (note: in our coordinate system this would be z instead of y but the notation was kept the same to follow convention)
# @param the x pixel resolution of the image we're taking: hres_x = 640
# @param the x field of view with respect to the camera from the intel realsense documentation: alpha_x = (pi * 63.4) / 180
# @param the y field of view with respect ot the camera from the intel realsense documentation: alpha_y =  (pi * 40.4) / 180 (note: in our coordinate system this would be z instead of y but the notation was kept the same to follow convention)
# @param the offset for the origin set to the starting servo of the robot: y arm offset = 841.375
# @param the y pixel on the screen of a given point: yp
#       Y
#       ^
#      |
#   X<--> Z
# In this coordinate system x and z are the horizontal and vertical coordinates with respect to the ground while y is the height coordinate.
# z is coming out of the robot 
def estCoordinateY(h, hres_y, beta, alpha_y, yp, offset):
    numerator = h * numpy.tan(alpha_y / 2) * (hres_y - yp)
    denominator = hres_y * numpy.tan(beta) + hres_y * numpy.tan(alpha_y / 2) - yp * numpy.tan(alpha_y / 2)
    return (numerator / denominator) - offset
# Returns the estimated x coordinate with respect to the camera. If you 
# want some other x value you need to calculate that offset. 
# @param beta pi / 2 - angle the camera makes between the mast: beta = 1.2117
# @param hres_y the y pixel resolution of the image we're taking: hres_y = 480 (note: in our coordinate system this would be z instead of y but the notation was kept the same to follow convention)
# @param hres_x the x pixel resolution of the image we're taking: hres_x = 640
# @param alpha_x the x field of view with respect to the camera from the intel realsense documentation: alpha_x = (pi * 63.4) / 180
# @param alpha_y the y field of view with respect ot the camera from the intel realsense documentation: alpha_y =  (pi * 40.4) / 180 (note: in our coordinate system this would be z instead of y but the notation was kept the same to follow convention)
# @param offset the offset for the origin set to the starting servo of the robot: x arm offset = 0
# @param yp the y pixel on the screen of a given point
# @param xp the x pixel on the screen of a given point
#       Y
#       ^
#      |
#   X<--> Z
# In this coordinate system x and z are the horizontal and vertical coordinates with respect to the ground while y is the height coordinate.
# z is coming out of the robot 
def estCoordinateX(h, hres_x, hres_y, beta, alpha_x, alpha_y, xp, yp, offset):
    numerator = h * hres_y * numpy.tan(alpha_x / 2) * (xp - hres_x)
    denominator = hres_x * (hres_y * numpy.tan(beta) + hres_y * numpy.tan(alpha_y / 2) - yp * numpy.tan(alpha_y / 2))
    return (numerator / denominator) - offset
# Returns the estimated z coordinate with respect to the camera. If you 
# want some other z value you need to calculate that offset. 
# @param pi / 2 - angle the camera makes between the mast: beta = 1.2117
# @param the y pixel resolution of the image we're taking: hres_y = 480 (note: in our coordinate system this would be z instead of y but the notation was kept the same to follow convention)
# @param the x pixel resolution of the image we're taking: hres_x = 640
# @param the y field of view with respect ot the camera from the intel realsense documentation: alpha_y =  (pi * 40.4) / 180 (note: in our coordinate system this would be z instead of y but the notation was kept the same to follow convention)
# @param the offset for the origin set to the starting servo of the robot: z arm offset = 53.975
# @param the y pixel on the screen of a given point: yp
#       Y
#       ^
#      |
#   X<--> Z
# In this coordinate system x and z are the horizontal and vertical coordinates with respect to the ground while y is the height coordinate.
# z is coming out of the robot 
def estCoordinateZ(h, hres_y, beta, alpha_y, yp, offset):
    numerator = h * hres_y
    denominator = hres_y * numpy.tan(beta) + hres_y * numpy.tan(alpha_y / 2) - yp * numpy.tan(alpha_y / 2)
    return (numerator / denominator) - offset
# returns the adjacent side of the triangle with respect to the camera, where 
# that side is parallel with the ground
# @param z coordinate taken from estCoordinateZ = z
# @param coordinate taken from estCoordinateX = z
def fndRadius(z, x):
    return int(10 * numpy.rint(numpy.sqrt(z**2 + x**2)))
# returns the theta value for the robotic arm which is in polar coordinates
# @param z coordinate taken from estCoordinateZ = z
# @param coordinate taken from estCoordinateX = z
def findTheta(z, x):
    return int(numpy.rint((numpy.arctan(z / x)) * (180 / numpy.pi)))
# returns the angle of rotation for robot
# @param adjacent side of the triangle
# @param hypotenuse side of the triangle
def findPsi(z, x):
    return int(numpy.rint((numpy.arccos(z / x)) * (180 / numpy.pi)))


# A small helper function for computing the sums of each quadrant 
# to obtain syringe orientation -JMC
def cmpCroppedColour(x_i,x_f,y_i,y_f,crop_colour):
	Q = 0
	for i in range(x_i,x_f):
		for j in range(y_i,y_f):
			Q += crop_colour[i][j]
	return Q
#           Q1 Q2 Q3
#           Q4 Q5 Q6
#           Q7 Q8 Q9

# Essentially if the sum of Q1 and Q9 is 
# greater than the sum of Q2 and Q8 and greater than the sum of Q3 and Q7 
# and greater than the sum of Q4 and Q6 we know the syringe mus be oriented at a 135 
# degree angle where zero is the syringe facing upward
# -JMC
def orientationCapture(x,y,w,h,img):

	crop_colour = img[y:y+h, x:x+w]
	gray = cv2.cvtColor(crop_colour, cv2.COLOR_BGR2GRAY)
	gray[gray < 144] = 0

	
	crop_colour = gray

	Q1 = 0
	Q2 = 0
	Q3 = 0
	Q4 = 0
	Q6 = 0
	Q7 = 0
	Q8 = 0
	Q9 = 0

	Q1 = cmpCroppedColour(0,int(numpy.rint(len(crop_colour)/3)),0,int(numpy.rint(len(crop_colour[0])/3)),crop_colour)
	Q2 = cmpCroppedColour(int(numpy.rint(len(crop_colour)/3) + 1), (int(2*numpy.rint(len(crop_colour)/3))), 0, int(numpy.rint(len(crop_colour[0])/3)), crop_colour)
	Q3 = cmpCroppedColour((int(2*numpy.rint(len(crop_colour))/3) + 1), int(numpy.rint(len(crop_colour))), 0, int(numpy.rint(len(crop_colour[0])/3)), crop_colour)
	Q4 = cmpCroppedColour(0, int(numpy.rint(len(crop_colour)/3)), int(numpy.rint(len(crop_colour[0])/3) + 1), (int(2*numpy.rint(len(crop_colour[0]))/3)), crop_colour)
	Q6 = cmpCroppedColour((int(2*numpy.rint(len(crop_colour))/3) + 1), int(numpy.rint(len(crop_colour))), int(numpy.rint(len(crop_colour[0])/3) + 1), (int(2*numpy.rint(len(crop_colour[0]))/3)), crop_colour)
	Q7 = cmpCroppedColour(0, int(numpy.rint(len(crop_colour)/3)), (2*int(numpy.rint(len(crop_colour[0]))/3) + 1), int(numpy.rint(len(crop_colour[0]))), crop_colour)
	Q8 = cmpCroppedColour(int(numpy.rint(len(crop_colour)/3) + 1), (int(2*numpy.rint(len(crop_colour))/3)), (int(2*numpy.rint(len(crop_colour[0]))/3) + 1), int(numpy.rint(len(crop_colour[0]))), crop_colour)
	Q9 = cmpCroppedColour((int(2*numpy.rint(len(crop_colour))/3) + 1), int(numpy.rint(len(crop_colour))), (int(2*numpy.rint(len(crop_colour[0]))/3) + 1), int(numpy.rint(len(crop_colour[0]))), crop_colour)

	degrees_0 = int(Q4 + Q6)
	degrees_45 = Q1 + Q9
	degrees_90 = Q2 + Q8
	degrees_135 = Q3 + Q7

	# first two if statements should take care of the issue of aspect ratio distortion.
	# That is if a bounding box is so narrow around the syringe we just assume the 0 or 90 degree 
	# case otherwise we do the summing of quadrants.
	if w * 2 < h: 
		return 90
	elif h * 2 < w:
		return 0
	else:
		if degrees_0 > degrees_45 and degrees_0 > degrees_90 and degrees_0 > degrees_135:
		    return 0
		if degrees_45 > degrees_0 and degrees_45 > degrees_90 and degrees_45 > degrees_135:
		    return 45
		if degrees_90 > degrees_0 and degrees_90 > degrees_45 and degrees_90 > degrees_135:
		    return 90
		if degrees_135 > degrees_0 and degrees_135 > degrees_90 and degrees_135 > degrees_45:
		    return 135


