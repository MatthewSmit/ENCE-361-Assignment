/**
 * @file pid.h
 *
 * @brief Generic pid controller module.
 */

#ifndef PID_H_
#define PID_H_

/**
 * A structure to accumulate the error and store the previous error for use by
 * the pid controller.
 */
typedef struct {
    /**
     * The previous error.
     */
    int32_t error_previous;

    /**
     * The accumulated error.
     */
    int32_t error_integrated;
} PidState;

/**
 * Initialise the pid controller.
 *
 * @param state The pid error state.
 * @see PidState
 */
void PidInit(PidState *state);

/**
 * Preload the integral component of the pid state with a postitve or negative error
 * to reduce integration time.
 *
 * @param state The pid error state.
 * @param integral_preload The preload error.
 */
void PreloadPid(PidState *state, int32_t integral_preload);

/**
 * Update the pid controller loop.
 *
 * @param state The pid error state.
 * @param error The current error.
 * @param delta_t The update period of the pid controller.
 * @param proportional_gain The proportional gain constant.
 * @param integral_gain The integral gain constant.
 * @param derivative_gain The derivative gain constant.
 * @return
 */
int32_t UpdatePid(PidState *state, int32_t error, uint32_t delta_t, double proportional_gain, double integral_gain,
        double derivative_gain);

#endif /* PID_H_ */
