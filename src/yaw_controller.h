/**
 * @file yaw_controller.h
 *
 * @brief Pid controller for the Tail rotor.
 */

/**
 * @defgroup yaw_controller YawController
 * @ingroup pid_api
 * @{
 */

#ifndef YAW_CONTROLLER_H_
#define YAW_CONTROLLER_H_

/**
 * Get the target yaw in degrees.
 *
 * @return The target yaw in degrees.
 */
int32_t GetTargetYawDegrees(void);

/**
 * Set the desired target yaw (degrees).
 *
 * @param yaw The desired target yaw (degrees).
 */
void SetTargetYawDegrees(int32_t yaw);

/**
 * Get the target yaw.
 *
 * @return the target yaw.
 * @see yaw_api for rotation unit.
 */
int32_t GetTargetYaw(void);

/**
 *
 * @param yaw
 */
void SetTargetYaw(int32_t yaw);

/**
 * Initialise the yaw controller.
 */
void YawControllerInit(void);

/**
 *
 * @param control
 * @param error
 */
void PreloadYawController(int32_t control, int32_t error);

/**
 *
 * @param delta_t
 */
void UpdateYawController(uint32_t delta_t);

/**
 * Use at own risk.
 *
 * @param gain
 */
void TuneProportionalTailRotor(double gain);

#endif /* YAW_CONTROLLER_H_ */

/** @} */
