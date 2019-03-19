#include <SensorLib.h>
#include <MotorLib.h>
#include <Syringenator.hpp>
#include <constants.h>
#include <LobotServoController.h>



void setup(){
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

	motorSetup();
	setupSensor_ISR(); //use this to set up timer interrupts for line sensors and distance sensors
	
	Serial.begin(SERIAL_BAUD); //For communication with the pi
	moveLineFollow();
}


void loop(){
		pivot_off_left();
		//delay(500);
			unsigned int temp = readLine_right();
			while(temp > 90){
			//Serial.println(temp);
			//	delayMicroseconds(500);
				temp = readLine_right();
			}
			stop_motors(); 
			pivot_off_right();
			temp = readLine_left();
			while(temp > 110){
			//Serial.println(temp);
			//	delayMicroseconds(500);
				temp = readLine_right();
			}
			delay(2000);
}


