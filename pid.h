/*
 * pid.h - A generic pid controller module.
 *
 *  Created on: 19/05/2017
 *      Author: daniel
 */

#ifndef PID_H_
#define PID_H_

typedef struct {
    int32_t error_previous;
    int32_t error_integrated;
} PidState;

void PidInit(PidState *state);
void PreloadPid(PidState *state, int32_t integral_preload);
int32_t UpdatePid(PidState *state, int32_t error, uint32_t delta_t, double proportional_gain, double integral_gain,
        double derivative_gain);

#endif /* PID_H_ */
