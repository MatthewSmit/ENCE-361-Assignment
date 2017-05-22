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

static double proportional_gain = 0.0;
static double integral_gain = 0.0;
static double derivative_gain = 0.0;

PidState height_state;
int32_t target_height;

void HeightControllerInit(void) {
    PidInit(&height_state);
}

void SetTargetHeight(uint32_t height) {
    /*
     * Check height in {0, 10, 20, ..., 100}
     */
	ASSERT(height % 10 == 0);
    ASSERT(height <= 100);

	target_height = height;
}

void UpdateHeightController(uint32_t delta_t) {
	int32_t height = GetHeightPercentage();
    int32_t error = target_height - height;
    int32_t control = UpdatePid(&height_state, error, delta_t, proportional_gain, integral_gain, derivative_gain);

    /* Clamp control inside valid range */
    control = (control < 2) ? 2 : (control > 98) ? 98 : control;
    SetPwmDutyCycle(MAIN_ROTOR, control);
}

void TuneParamMainRotor(double k_p1, double k_i1, double k_d1) {
	proportional_gain = k_p1;
	integral_gain = k_i1;
	derivative_gain = k_d1;
}
