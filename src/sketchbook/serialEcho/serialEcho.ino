#include <constants.h>

#define IN_BUF_SZ 8

void setup(){
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

	Serial.begin(SERIAL_BAUD);
}



char serialDataIn[IN_BUF_SZ];
int bytesRead=0;
void serialEvent(){
	bytesRead = Serial.readBytesUntil(ARDUINO_NULL, serialDataIn, IN_BUF_SZ);
}

void loop(){
	
	if(bytesRead){
		for(int i=0; i<bytesRead; i++)
			Serial.write(serialDataIn[i]);
		bytesRead =0;
		
		digitalWrite(LED_BUILTIN, HIGH);
		delay(100);
		digitalWrite(LED_BUILTIN, LOW);
	}
}
