

import serial
import constants


class ComPort:
	def __init__(self):
		#self.p = serial.Serial('/dev/ttyUSB0', timeout=1)
		self.p = serial.Serial('/dev/ttyACM0', timeout=1)
	def status(self):
		if self.p.in_waiting == 0:
			return constants.ARDUINO_NULL
		bytes = bytearray(self.p.read())
		return bytes[0]
	
	def send(self, bytes):
		length = len(bytes)
		
		sentLength = self.p.write(bytes)
		
		if(length != sentLength): print "SySerial: length mismatch"
		

def statusString(s):
	table = {
		constants.ARDUINO_NULL               : "ARDUINO_NULL",
		constants.ARDUINO_STATUS_ACK         : "ARDUINO_STATUS_ACK",
		constants.ARDUINO_STATUS_READY       : "ARDUINO_STATUS_READY",
		constants.ARDUINO_STATUS_PICK_FAIL   : "ARDUINO_STATUS_PICK_FAIL",
		constants.ARDUINO_STATUS_PICK_SUCCESS: "ARDUINO_STATUS_PICK_SUCCESS",
		constants.ARDUINO_STATUS_ARM_FAULT   : "ARDUINO_STATUS_ARM_FAULT",
		constants.ARDUINO_STATUS_OBSTACLE    : "ARDUINO_STATUS_OBSTACLE",
		constants.ARDUINO_ROTATE             : "ARDUINO_ROTATE",
		constants.ARDUINO_MOVE               : "ARDUINO_MOVE",
		constants.ARDUINO_LINE_FOLLOW        : "ARDUINO_LINE_FOLLOW",
		constants.ARDUINO_AVOID              : "ARDUINO_AVOID",
		constants.ARDUINO_RETURN             : "ARDUINO_RETURN",
		constants.ARDUINO_ARM_PARK           : "ARDUINO_ARM_PARK",
		constants.ARDUINO_ARM_DISPOSE        : "ARDUINO_ARM_DISPOSE",
		constants.ARDUINO_ARM_PICKUP         : "ARDUINO_ARM_PICKUP"
	}
	return table.get(s, "INVALID CODE")


