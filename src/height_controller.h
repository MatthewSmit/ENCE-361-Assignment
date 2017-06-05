/**
 * @file height_controller.h
 *
 * @brief Pid controller for the Main rotor.
 */

/**
 * @defgroup height_controller HeightController
 * @ingroup pid_api
 * @{
 */

#ifndef HEIGHT_CONTROLLER_H_
#define HEIGHT_CONTROLLER_H_

/**
 * Set the target height (%).
 *
 * @param height The target height (%).
 */
void SetTargetHeight(uint32_t height);

/**
 * Get the target height (%).
 *
 * @return The target height(%).
 */
uint32_t GetTargetHeight(void);

/**
 * Initialise the height controller.
 */
void HeightControllerInit(void);

/**
 * Preload the integral component of the pid contoller so the Main rotor starts of
 * with @p control power.
 *
 * This was to improves rise time of the helicopter by boosting the Main rotor.
 *
 * @param control The immediate control power desired by the Main rotor
 * @param error The absolute difference between current height and target height.
 */
void PreloadHeightController(int32_t control, int32_t error);

/**
 * Update the height controller pid loop.
 *
 * @param delta_t The update period of the height controller.
 */
void UpdateHeightController(uint32_t delta_t);

/**
 * Use at own risk.
 *
 * @param gain Proportial gain.
 */
void TuneProportionalMainRotor(double gain);

#endif /* HEIGHT_CONTROLLER_H_ */

/** @} */
