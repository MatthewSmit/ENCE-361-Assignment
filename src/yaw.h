/**
 * @file yaw.h
 *
 * Handles the all of the yaw and yaw reference detection.
 */

#ifndef YAW_H_
#define YAW_H_

/*
 * The number of slots in 360 degrees of rotation.
 */
#define NUMBER_SLOTS            112

/*
 * The number of yaw updates in 360 degrees of rotation.
 */
#define YAW_FULL_ROTATION       (NUMBER_SLOTS * 4)

/**
 * Initialises the yaw manager.
 */
void YawDetectionInit(void);

/**
 * Get the current yaw.
 *
 * @return the yaw (notches)
 */
int32_t GetYaw(void);

/**
 * Get the current yaw.
 *
 * @return the yaw (degrees)
 */
int32_t GetYawDegrees(void);

/**
 * Helper function to return the closest yaw such that the helicopter is facing towards the camera.
 *
 * @param current_yaw the current yaw
 * @return the closest reference yaw
 */
int32_t GetClosestYawRef(int32_t current_yaw);

/**
 * Triggers an interrupt to fire when the refernce yaw has been found.
 */
void YawRefTrigger(void);

/**
 * Check if the reference yaw has been found.
 *
 * @return true if the yaw reference has been found else false
 */
bool YawRefFound(void);

#endif /* YAW_H_ */
