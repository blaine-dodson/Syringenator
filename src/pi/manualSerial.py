import SySerial as ss
import constants

port = ss.ComPort()


print "begin manual control"


while(True):
	
	port.waitForReady()
	print "arduino ready"
	
	# read some numbers from command line
	inStr = raw_input("enter bytes: ")
	byteStrings = inStr.split()
	
	bytesOut = []
	
	for s in byteStrings:
		bytesOut.append(int(s))
	
	# send numbers to serial
	port.send(bytesOut)
	print "bytes sent"
	
	
	# in all cases we wait for the arduino to be ready
	status = port.status()
	print "status is: " + ss.statusString(status)

