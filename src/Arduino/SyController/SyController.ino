

#include <SensorLib.h>
#include <MotorLib.h>
#include <Syringenator.hpp>
#include <constants.h>
#include <LobotServoController.h>


#define IN_BUF_SZ 8

uint8_t serialDataIn[IN_BUF_SZ];
int bytesRead=0;

void serialEvent(){
	bytesRead = Serial.readBytes(serialDataIn, IN_BUF_SZ);
}

void errorBlink(void){
	digitalWrite(LED_BUILTIN, LOW);
	for(int i=0; i< 10; i++){
		delay(300);
		digitalWrite(LED_BUILTIN, HIGH);
		delay(300);
		digitalWrite(LED_BUILTIN, LOW);
	}
}


void parseCommand(uint8_t * bytes, int cnt){
	switch(bytes[0]){
	case ARDUINO_LINE_FOLLOW:
		if(cnt == 1){
			Serial.write(ARDUINO_STATUS_ACK);
			moveLineFollow();
		}else
			Serial.write(ARDUINO_STATUS_NACK);
		break;
	
	case ARDUINO_RETURN:
		if(cnt == 1){
			Serial.write(ARDUINO_STATUS_ACK);
			deadReckoning();
		}else
			Serial.write(ARDUINO_STATUS_NACK);
		break;
	
	case ARDUINO_FWD:
		if(cnt == 2){
			Serial.write(ARDUINO_STATUS_ACK);
			moveStraight(bytes[1]);
		}else
			Serial.write(ARDUINO_STATUS_NACK);
		break;
	
	case ARDUINO_RIGHT:
		if(cnt == 2){
			Serial.write(ARDUINO_STATUS_ACK);
			moveRotate(ARDUINO_RIGHT, bytes[1]);
		}else
			Serial.write(ARDUINO_STATUS_NACK);
		break;
	
	case ARDUINO_LEFT:
		if(cnt == 2){
			Serial.write(ARDUINO_STATUS_ACK);
			moveRotate(ARDUINO_LEFT, bytes[1]);
		}else
			Serial.write(ARDUINO_STATUS_NACK);
		break;
	
	case ARDUINO_ARM_PICKUP:
		if(cnt == 4){
			Serial.write(ARDUINO_STATUS_ACK);
			grabObject(bytes[1], bytes[2], bytes[3]);
		}else 
			Serial.write(ARDUINO_STATUS_NACK);
		
		break;
	
	case ARDUINO_AVOID:
	case ARDUINO_ARM_PARK:
	case ARDUINO_ARM_DISPOSE:
	default:
		Serial.write(ARDUINO_STATUS_NACK);
	}
	while(!isReady());
}


void setup(){
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

	motorSetup();
	setupSensor_ISR(); //use this to set up timer interrupts for line sensors and distance sensors
	
	Serial.begin(SERIAL_BAUD); //For communication with the pi
	Serial1.begin(9600); //For communication with the xarm
}


void loop(){
	if(bytesRead){
		digitalWrite(LED_BUILTIN, HIGH);
		parseCommand(serialDataIn, bytesRead);
		bytesRead =0;
	}
	else{
		digitalWrite(LED_BUILTIN, LOW);
		Serial.write(ARDUINO_STATUS_READY);
		delay(300);
	}
}


