

import SySerial as ss

port = ss.ComPort()


print("start demo")


while(True):
	# read some numbers from command line
	inStr = raw_input("enter bytes: ")
	byteStrings = inStr.split()
	
	bytes = []
	
	for s in byteStrings:
		bytes.append(int(s))
	
	# send numbers to serial
	port.send(bytes)
	print "demoSerial: bytes sent"
	
	# read results from serial
	

status = None
while(status == None):
	status = port.status()

print "status is: " + ss.statusString(status)

while(status != None):
	status = port.status()


