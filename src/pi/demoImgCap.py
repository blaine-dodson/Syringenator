

import cv2
import pyrealsense2 as rs

pipeline = rs.pipeline()

try:
	# Create a context object. This object owns the handles to all connected
	# realsense devices
	pipeline.start()
except:
	print("Camera not found")
	exit()




