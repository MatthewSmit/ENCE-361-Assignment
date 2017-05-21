/*
 * yaw_controller.c - Yaw Controller.
 */

#include <stdbool.h>
#include <stdint.h>
#include "utils/uartstdio.h"
#include "driverlib/debug.h"

#include "yaw_manager.h"
#include "yaw_controller.h"

//static double k_p = 5.0 * 0.45;
//static double k_i = 0.5 / 1.2 / 1000;
//static double k_d = 0.0;

static double proportional_gain = 2.0;
static double integral_gain = 0.0;
static double derivative_gain = 0.0;

PidState yaw_state;
int32_t target_yaw;

void YawControllerInit(void) {
    PidInit(&yaw_state);
}

void SetTargetYaw(int32_t yaw) {
    ASSERT(yaw <= 360);
    ASSERT(yaw >= -360);

    target_yaw = yaw;
}

void UpdateYawController(uint32_t delta_t) {
    int32_t yaw = GetYaw();
    int32_t error = target_yaw - yaw;
    int32_t control = UpdatePid(&yaw_state, error, delta_t, proportional_gain, integral_gain, derivative_gain);
    control = (control < 2) ? 2 : (control > 98) ? 98 : control;
    SetPwmDutyCycle(TAIL_ROTOR, control);
}

void TuneParamTailRotor(double my_k_p, double my_k_i, double my_k_d) {
    proportional_gain = my_k_p;
    integral_gain = my_k_i;
    derivative_gain = my_k_d;
}

