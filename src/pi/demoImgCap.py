

FRAME_RATE = 30

import cv2
import numpy as np
import pyrealsense2 as rs

pipeline = rs.pipeline()

cfg = rs.config()

#enable_stream(stream_type: rs.stream, width: int, height: int, format: rs.format=format.any, framerate: int=0L) -> None
cfg.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, FRAME_RATE)
#config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, FRAME_RATE)

try:
	# Create a context object. This object owns the handles to all connected
	# realsense devices
	pipeline.start(cfg)
except:
	print("Camera not found")
	exit()

for i in range(0,30):
	frames = pipeline.wait_for_frames()


frame = frames.get_color_frame()

data = frame.get_data()

dir(data)
data.__doc__

#mat = np.frombuffer(frame.get_data(), np.uint8)
mat = np.array(data, np.uint8)

mat2 = np.reshape(mat, (480, 640, 3))


cv2.namedWindow("Display Image", cv2.WINDOW_AUTOSIZE );
cv2.imshow("Display Image", mat2);

cv2.waitKey(0);

#mat = cv2.CreateMat(cv2.Size(640,480), CV_8UC3, , Mat_AUTO_STEP)

pipeline.stop()

