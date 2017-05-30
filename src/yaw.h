/**
 * @file yaw.h
 *
 * Handles the low level yaw detection.
 */

#ifndef YAW_H_
#define YAW_H_

/**
 * The number of slots in 360 degrees of rotation.
 */
#define NUMBER_SLOTS            112
/**
 * The number of yaw updates in 360 degrees of rotation.
 */
#define YAW_FULL_ROTATION       (NUMBER_SLOTS * 4)

/**
 * Initialises the yaw manager.
 */
void YawDetectionInit(void);
/**
 * Returns the current yaw in notches.
 */
int32_t GetYaw(void);
/**
 * Returns the current yaw in degrees.
 */
int32_t GetYawDegrees(void);
/**
 * Returns the closest reference yaw to the actual yaw value.
 */
int32_t GetClosestYawRef(int32_t yaw);
/**
 * Enables the reference yaw trigger interrupt.
 */
void YawRefTrigger(void);
/**
 * Returns true if the reference yaw has been found.
 */
bool YawRefFound(void);

#endif /* YAW_H_ */
