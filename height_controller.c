/**
 * @file HeightController.c

 * @author Daniel van Wichen
 */

#include <stdbool.h>
#include <stdint.h>
#include "utils/uartstdio.h"
#include "driverlib/debug.h"

#include "height_controller.h"
#include "height.h"

static const double ultimate_gain = 1.0;
static const double period = 700.0;

static double integral_time;
static double derivative_time;
static double proportional_gain;
static double integral_gain;
static double derivative_gain;

static PidState height_state;
static uint32_t target_height;

void HeightControllerInit(void) {
//    integral_time = period / 2.0;
//    derivative_time = period / 3.0;
//
//    proportional_gain = ultimate_gain * 0.33;
    integral_time = period * 2.2;
    derivative_time = period / 6.3;

    proportional_gain = ultimate_gain / 2.2;
    integral_gain = proportional_gain / integral_time;
    derivative_gain = proportional_gain * derivative_time;

    PidInit(&height_state);
}

void SetTargetHeight(uint32_t height) {
    ASSERT(height <= 100);

	target_height = height;
}

uint32_t GetTargetHeight(void) {
    return target_height;
}

void UpdateHeightController(uint32_t delta_t) {
	int32_t height = GetHeightPercentage();
    int32_t error = (int32_t) target_height - height;
    int32_t control = UpdatePid(&height_state, error, delta_t,
            proportional_gain, integral_gain, derivative_gain);

    /* Clamp control inside valid range */
    control = (control < 15) ? 15 : (control > 98) ? 98 : control;
    SetPwmDutyCycle(MAIN_ROTOR, control);
}

void TuneParamMainRotor(double k_p1, double k_i1, double k_d1) {
    proportional_gain = k_p1;
    integral_time = k_i1;
    derivative_time = k_d1;
}
