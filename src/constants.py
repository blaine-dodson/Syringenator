
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


