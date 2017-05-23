/*
 * yaw_controller.c - Yaw Controller.
 */

#include <stdbool.h>
#include <stdint.h>
#include "utils/uartstdio.h"
#include "driverlib/debug.h"

#include "yaw.h"
#include "yaw_controller.h"

static const double ultimate_gain = 2.4;
static const double period = 1000.0;

static double integral_time;
static double derivative_time;
static double proportional_gain;
static double integral_gain;
static double derivative_gain;

static PidState yaw_state;
static int32_t target_yaw_degrees;
static int32_t target_yaw;

void YawControllerInit(void) {
    integral_time = 2.2 * period;
    derivative_time = period / 6.3;

    proportional_gain = ultimate_gain / 2.2;
    integral_gain = proportional_gain / integral_time;
    derivative_gain = proportional_gain * derivative_time;

    PidInit(&yaw_state);
}

void SetTargetYawDegrees(int32_t yaw) {
    target_yaw_degrees = yaw;
    target_yaw = target_yaw_degrees * YAW_FULL_ROTATION / 360;
}

int32_t GetTargetYawDegrees(void) {
    return target_yaw_degrees;
}

int32_t GetTargetYaw(void) {
    return target_yaw;
}

void SetTargetYaw(int32_t yaw) {
    target_yaw = yaw;
    target_yaw_degrees = target_yaw * 360 / YAW_FULL_ROTATION;
}

void UpdateYawController(uint32_t delta_t) {
    int32_t yaw = GetYaw();
    int32_t error = target_yaw - yaw;
    int32_t control = UpdatePid(&yaw_state, error, delta_t, proportional_gain,
            integral_gain, derivative_gain);
    // TODO SetContraints(min, max)
    control = (control < 2) ? 2 : (control > 98) ? 98 : control;
    SetPwmDutyCycle(TAIL_ROTOR, control);
}

void TuneParamTailRotor(double my_k_p, double my_k_i, double my_k_d) {
    proportional_gain = my_k_p;
    integral_gain = my_k_i;
    derivative_gain = my_k_d;
}

