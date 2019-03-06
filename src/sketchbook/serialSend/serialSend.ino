#include <constants.h>


void setup(){
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.begin(SERIAL_BAUD);
}

void loop(){
	digitalWrite(LED_BUILTIN, HIGH);
	Serial.write(ARDUINO_STATUS_READY);
	delay(1000);
	digitalWrite(LED_BUILTIN, LOW);
	Serial.write(ARDUINO_STATUS_READY);
	delay(1000);
}
