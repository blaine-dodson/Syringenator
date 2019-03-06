

import SySerial as ss
import constants

port = ss.ComPort()


print("start demo")


status = constants.ARDUINO_NULL
while(status == constants.ARDUINO_NULL):
	status = port.status()
	print("status is ",status )

if status == constants.ARDUINO_STATUS_READY:
	print("arduino ready")
else:
	print "arduino not ready"
