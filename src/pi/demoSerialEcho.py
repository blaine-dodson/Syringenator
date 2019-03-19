

import SySerial as ss
import constants

port = ss.ComPort()


print("start demo")


while(True):
#	while(port.status() != constants.ARDUINO_STATUS_READY):
#		pass
	
	# read some numbers from command line
	inStr = raw_input("enter bytes: ")
	byteStrings = inStr.split()
	
	bytesOut = []
	
	for s in byteStrings:
		bytesOut.append(int(s))
	
	# send numbers to serial
	port.send(bytesOut)
	print "demoSerial: bytes sent"
	
	# read results from serial
	bytesIn = []
	
	byte = constants.ARDUINO_NULL
	while(byte == constants.ARDUINO_NULL):
		byte = port.status()

	print "status is: " + ss.statusString(byte)

	bytesIn.append(byte)
	
	byte = port.status()
	while(byte != constants.ARDUINO_NULL):
		print byte
		bytesIn.append(byte)
		
		byte = port.status()
	
	if bytesIn == bytesOut:
		print "exact echo"
	else:
		print "bad echo"


