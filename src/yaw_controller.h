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
 * Set the desired target yaw.
 *
 * @param yaw The desired target yaw.
 */
void SetTargetYaw(int32_t yaw);

/**
 * Initialise the yaw controller.
 */
void YawControllerInit(void);

/**
 * Preload the integral component of the pid contoller so the Tail rotor starts of
 * with @p control power.
 *
 * Can be used in conjuction with @ref PreloadHeightController to improve the
 * rise time of the helicopter.
 *
 * @param control The immediate control power desired by the Tail rotor
 * @param error The absolute difference between current yaw and target yaw.
 */
void PreloadYawController(int32_t control, int32_t error);

/**
 * Update the yaw controller pid loop.
 *
 * @param delta_t The update period of the yaw controller.
 */
void UpdateYawController(uint32_t delta_t);

/**
 * Use at own risk.
 *
 * @param gain Proportial gain.
 */
void TuneProportionalTailRotor(double gain);

#endif /* YAW_CONTROLLER_H_ */

/** @} */
