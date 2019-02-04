
#define __SOMETHING /// Defines the limits of the parameters passed to the armPick() routine. They define the limits on the range of motion of the arm during a pickup.
#define ARM_AZIMUTH_MIN 0
#define ARM_AZIMUTH_MAX 0
#define ARM_RANGE_MIN   0
#define ARM_RANGE_MAX   0
#define ARM_ORIENT_MIN  0
#define ARM_ORIENT_MAX  0

#ifdef __NOTHING
ARM_AZIMUTH_MIN = 0
ARM_AZIMUTH_MAX = 0
ARM_RANGE_MIN   = 0
ARM_RANGE_MAX   = 0
ARM_ORIENT_MIN  = 0
ARM_ORIENT_MAX  = 0
#endif





#define __SOMETHING /// Defines the limits of the arm pickup area. these values must be in the same units as the bounding box data of the Target object
#define PICKUP_X_MIN 0
#define PICKUP_X_MAX 0
#define PICKUP_Y_MIN 0
#define PICKUP_Y_MAX 0

#ifdef __NOTHING
PICKUP_X_MIN = 0
PICKUP_X_MAX = 0
PICKUP_Y_MIN = 0
PICKUP_Y_MAX = 0
#endif





#define __SOMETHING /// defines status signals from the arduino to the pi
#define ARDUINO_NULL                0x00
#define ARDUINO_STATUS_ACK          0x01
#define ARDUINO_STATUS_READY        0x02
#define ARDUINO_STATUS_PICK_FAIL    0x03
#define ARDUINO_STATUS_PICK_SUCCESS 0x04
#define ARDUINO_STATUS_ARM_FAULT    0x05
#define ARDUINO_STATUS_OBSTACLE     0x06

#define __SOMETHING /// defines robot locomotion commands
#define ARDUINO_ROTATE              0x10
#define ARDUINO_MOVE                0x11
#define ARDUINO_LINE_FOLLOW         0x12

#define __SOMETHING /// defines arm commands
#define ARDUINO_ARM_PARK            0x20
#define ARDUINO_ARM_DISPOSE         0x21
#define ARDUINO_ARM_PICKUP          0x22

#ifdef __NOTHING
ARDUINO_NULL                =0x00
ARDUINO_STATUS_ACK          =0x01
ARDUINO_STATUS_READY        =0x02
ARDUINO_STATUS_PICK_FAIL    =0x03
ARDUINO_STATUS_PICK_SUCCESS =0x04
ARDUINO_STATUS_ARM_FAULT    =0x05
ARDUINO_STATUS_OBSTACLE     =0x06

ARDUINO_ROTATE              =0x10
ARDUINO_MOVE                =0x11
ARDUINO_LINE_FOLLOW         =0x12

ARDUINO_ARM_PARK            =0x20
ARDUINO_ARM_DISPOSE         =0x21
ARDUINO_ARM_PICKUP          =0x22
#endif




#define __SOMETHING /// Defines arduino pins
#define PORT_MOTOR_FWD
#define PORT_MOTOR_AFT
#define STBD_MOTOR_FWD
#define STBD_MOTOR_AFT

#define PORT_LINE_SENSE
#define STBD_LINE_SENSE

#define PORT_FWD_OBSTACLE
#define PORT_AFT_OBSTACLE
#define STBD_FWD_OBSTACLE
#define STBD_AFT_OBSTACLE

#define ARM_CONTROL



