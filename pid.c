/**
 * @file pid.c
 * @date
 * @brief A generic pid controller module.
 *
 *
 */

#include "stdint.h"
#include "pid.h"

void PidInit(PidState *state) {
    state->error_previous = 0;
    state->error_integrated = 0;
}

int32_t UpdatePid(PidState *state, int32_t error, uint32_t delta_t, double proportional_gain, double integral_gain,
        double derivative_gain) {

    state->error_integrated += (int32_t) delta_t * error;
    int32_t error_integrated = state->error_integrated;
    double error_derivative = (double) (error - state->error_previous) / delta_t;

    state->error_previous = error;

    int32_t control = error * proportional_gain + error_integrated * integral_gain + error_derivative * derivative_gain;
    return control;
}
