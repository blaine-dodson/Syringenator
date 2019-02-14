/******************************************************
* FileName:      LobotServoController.h
* Company:     Lewan Soul
* Date:          2016/07/02  16:53
 *Last Modification Date: 201706281638
* www.lewansoul.com
*****************************************************/

#ifndef LOBOTSERVOCONTROLLER_H
#define LOBOTSERVOCONTROLLER_H

#include <Arduino.h>
#include <SoftwareSerial.h>

#define FRAME_HEADER            0x55
#define CMD_SERVO_MOVE          0x03
#define CMD_ACTION_GROUP_RUN    0x06
#define CMD_ACTION_GROUP_STOP   0x07
#define CMD_ACTION_GROUP_SPEED  0x0B
#define CMD_GET_BATTERY_VOLTAGE 0x0F 
#define CMD_MULT_SERVO_POS_READ 0x15


#define BATTERY_VOLTAGE       0x0F  
#define ACTION_GROUP_RUNNING  0x06
#define ACTION_GROUP_STOPPED  0x07
#define ACTION_GROUP_COMPLETE 0x08
#define SERVO_ID_1            0x01
#define SERVO_ID_2            0x02
#define SERVO_ID_3            0x03
#define SERVO_ID_4            0x04
#define SERVO_ID_5            0x05
#define SERVO_ID_6            0x06


struct LobotServo { 
  uint8_t  ID; 
  uint16_t Position;
};

class LobotServoController {
  public:
    LobotServoController(SoftwareSerial &A);
	LobotServoController(HardwareSerial &A);
    void moveServo(uint8_t servoID, uint16_t Position, uint16_t Time);
    void moveServos(LobotServo servos[], uint8_t Num, uint16_t Time);
    void moveServos(uint8_t Num,uint16_t Time, ...);
    void runActionGroup(uint8_t NumOfAction, uint16_t Times);
    void stopActionGroup(void);
    void setActionGroupSpeed(uint8_t NumOfAction, uint16_t Speed);
    void setAllActionGroupSpeed(uint16_t Speed);

    uint16_t * readServos();
    uint16_t getBatteryVolt(void);
    uint16_t getBatteryVolt(uint32_t timeout);
    bool waitForStopping(uint32_t timeout);
    void sendCMDGetBatteryVolt(void);
    void sendCMDGetServoPositions(void);
	bool isRunning(void);
	void receiveHandle(void);
	
  public:
    uint8_t  numOfActinGroupRunning; 
    uint16_t actionGroupRunTimes;
    
  private:
	bool isUseHardwareSerial;
	bool isGetBatteryVolt;
  bool isReadServos;
  uint16_t batteryVoltage;
	bool isRunning_;
  Stream *SerialX;
  uint16_t *servoPos;
};
#endif
