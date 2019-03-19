

# Realsense Parameters
FRAME_RATE = 30
IMG_WIDTH  = 640
IMG_HEIGHT = 480

SAMPLES = 300

import pyrealsense2 as rs


pipeline = rs.pipeline()

cfg = rs.config()

#enable_stream(stream_type: rs.stream, width: int, height: int, format: rs.format=format.any, framerate: int=0L) -> None
cfg.enable_stream(rs.stream.depth, IMG_WIDTH, IMG_HEIGHT, rs.format.z16, FRAME_RATE)
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

maximum = 0
minimum = 10000000
avg = 0


for i in range(1,SAMPLES):
	frames = pipeline.wait_for_frames()
	frame = frames.get_depth_frame()
	if not frame: continue
	
	dist = frame.get_distance(IMG_WIDTH/2, IMG_HEIGHT/2)
	
	print(dist)
	
	avg += dist
	if dist > maximum: maximum = dist
	if dist < minimum: minimum = dist

avg /= SAMPLES

print("max: "+str(maximum)+" min: "+str(minimum)+" avg: "+str(avg))



