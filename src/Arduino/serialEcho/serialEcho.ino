#include <constants.h>

#define IN_BUF_SZ 8

void setup(){
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

	Serial.begin(SERIAL_BAUD);
}



uint8_t serialDataIn[IN_BUF_SZ];
int bytesRead=0;
void serialEvent(){
	bytesRead = Serial.readBytesUntil(ARDUINO_NULL, serialDataIn, IN_BUF_SZ);
}

void loop(){
	if(bytesRead){
		digitalWrite(LED_BUILTIN, HIGH);
		
		switch(serialDataIn[0]){
		case ARDUINO_FWD:
			Serial.write(ARDUINO_STATUS_ACK);
		default:
			Serial.write(ARDUINO_STATUS_NACK);
		}
		Serial.flush();
		
		bytesRead =0;
		//delay(100);
		digitalWrite(LED_BUILTIN, LOW);
	}
	else{
		Serial.write(ARDUINO_STATUS_READY);
		delay(500);
	}
}
