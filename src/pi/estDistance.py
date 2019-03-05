import numpy
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
    hypotenuse = h * numpy.tan(alpha_y / 2) * (hres_y - yp)
    denominator = hres_y * numpy.tan(beta) + hres_y * numpy.tan(alpha_y / 2) - yp * numpy.tan(alpha_y / 2)
    return (hypotenuse / denominator) - offset
# Returns the estimated x coordinate with respect to the camera. If you 
# want some other x value you need to calculate that offset. 
# @param pi / 2 - angle the camera makes between the mast: beta = 1.2117
# @param the y pixel resolution of the image we're taking: hres_y = 480 (note: in our coordinate system this would be z instead of y but the notation was kept the same to follow convention)
# @param the x pixel resolution of the image we're taking: hres_x = 640
# @param the x field of view with respect to the camera from the intel realsense documentation: alpha_x = (pi * 63.4) / 180
# @param the y field of view with respect ot the camera from the intel realsense documentation: alpha_y =  (pi * 40.4) / 180 (note: in our coordinate system this would be z instead of y but the notation was kept the same to follow convention)
# @param the offset for the origin set to the starting servo of the robot: x arm offset = 0
# @param the y pixel on the screen of a given point: yp
# @param the x pixel on the screen of a given point: xp
#       Y
#       ^
#      |
#   X<--> Z
# In this coordinate system x and z are the horizontal and vertical coordinates with respect to the ground while y is the height coordinate.
# z is coming out of the robot 
def estCoordinateX(h, hres_x, hres_y, beta, alpha_x, alpha_y, xp, yp, offset):
    hypotenuse = h * hres_y * numpy.tan(alpha_x / 2) * (xp - hres_x)
    denominator = hres_x * (hres_y * numpy.tan(beta) + hres_y * numpy.tan(alpha_y / 2) - yp * numpy.tan(alpha_y / 2))
    return (hypotenuse / denominator) - offset
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
    hypotenuse = h * hres_y
    denominator = hres_y * numpy.tan(beta) + hres_y * numpy.tan(alpha_y / 2) - yp * numpy.tan(alpha_y / 2)
    return (hypotenuse / denominator) - offset
# returns the adjacent side of the triangle with respect to the camera, where 
# that side is parallel with the ground
# @param z coordinate taken from estCoordinateZ = z
# @param coordinate taken from estCoordinateX = z
def fndRadius(z, x):
    return int(10 * numpy.rint(numpy.sqrt(z^2 + x^2)))
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