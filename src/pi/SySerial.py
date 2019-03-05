

import serial


class ComPort:
	def __init__(self):
		self.p = serial.Serial('/dev/ttyUSB0', timeout=1)
	
	def status(self):
		if self.p.in_waiting == 0:
			return None
		bytes = bytearray(self.p.read())
		return bytes[0]
	
	def send(self, count, *bytes):
		self.p.write(bytes)


