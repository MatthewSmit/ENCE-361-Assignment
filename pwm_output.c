/**
 *
 */

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

#include "pwm_output.h"

/* PWM Main Rotor Definitions */
#define PWM_MAIN_BASE           PWM0_BASE
#define PWM_MAIN_GEN            PWM_GEN_3
#define PWM_MAIN_OUTNUM         PWM_OUT_7
#define PWM_MAIN_OUTBIT         PWM_OUT_7_BIT
#define PWM_MAIN_PERIPH_PWM     SYSCTL_PERIPH_PWM0
#define PWM_MAIN_PERIPH_GPIO    SYSCTL_PERIPH_GPIOC
#define PWM_MAIN_GPIO_BASE      GPIO_PORTC_BASE
#define PWM_MAIN_GPIO_CONFIG    GPIO_PC5_M0PWM7
#define PWM_MAIN_GPIO_PIN       GPIO_PIN_5

/* PWM Tail Rotor Definitions */
#define PWM_TAIL_BASE           PWM1_BASE
#define PWM_TAIL_GEN            PWM_GEN_2
#define PWM_TAIL_OUTNUM         PWM_OUT_5
#define PWM_TAIL_OUTBIT         PWM_OUT_5_BIT
#define PWM_TAIL_PERIPH_PWM     SYSCTL_PERIPH_PWM1
#define PWM_TAIL_PERIPH_GPIO    SYSCTL_PERIPH_GPIOF
#define PWM_TAIL_GPIO_BASE      GPIO_PORTF_BASE
#define PWM_TAIL_GPIO_CONFIG    GPIO_PF1_M1PWM5
#define PWM_TAIL_GPIO_PIN       GPIO_PIN_1

#define PWM_DIVIDER_CODE        SYSCTL_PWMDIV_16
#define PWM_DIVIDER             16

static bool pwm_state[2];

bool GetPwmState(uint8_t pwm_output);

void PwmInit() {
    SysCtlPWMClockSet(PWM_DIVIDER_CODE);

    uint32_t period = SysCtlClockGet() / PWM_DIVIDER / PWM_FREQUENCY;

    /* Initialise Main Rotor */
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM);

    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);

    PwmDisable(MAIN_ROTOR);

    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);

    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, period);
    SetPwmDutyCycle(MAIN_ROTOR, 2);

    /* Initialise Tail Rotor */
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_GPIO);
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_PWM);

    GPIOPinConfigure(PWM_TAIL_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_TAIL_GPIO_BASE, PWM_TAIL_GPIO_PIN);

    PwmDisable(TAIL_ROTOR);

    PWMGenConfigure(PWM_TAIL_BASE, PWM_TAIL_GEN, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenEnable(PWM_TAIL_BASE, PWM_TAIL_GEN);

    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, period);
    SetPwmDutyCycle(TAIL_ROTOR, 2);
}

void SetPwmDutyCycle(uint8_t pwm_output, uint32_t duty_cycle) {
	ASSERT(!(duty_cycle < 2 || duty_cycle > 98));
	uint32_t period = SysCtlClockGet() / PWM_DIVIDER / PWM_FREQUENCY;
	switch (pwm_output) {
	case MAIN_ROTOR:
		PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM,
				period * duty_cycle / 100);
		break;
	case TAIL_ROTOR:
		PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM,
				period * duty_cycle / 100);
		break;
	}
}

uint32_t GetPwmDutyCycle(uint8_t pwm_output) {
    uint32_t duty_cycle;
    uint32_t period = SysCtlClockGet() / PWM_DIVIDER / PWM_FREQUENCY;
    if (pwm_state[pwm_output]) {
        switch (pwm_output) {
        case MAIN_ROTOR:
            duty_cycle = PWMPulseWidthGet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM) * 100
                    / period;
            break;
        case TAIL_ROTOR:
            duty_cycle = PWMPulseWidthGet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM) * 100
                    / period;
            break;
        }
    } else {
        duty_cycle = 0;
    }
    return duty_cycle;
}

void SetPwmState(uint8_t pwm_output, bool state) {
	switch (pwm_output) {
	case MAIN_ROTOR:
		PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, state);
        pwm_state[MAIN_ROTOR] = state;
		break;
	case TAIL_ROTOR:
		PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, state);
        pwm_state[TAIL_ROTOR] = state;
		break;
	}
}

bool GetPwmState(uint8_t pwm_output) {
    return pwm_state[pwm_output];
}

void PwmEnable(uint8_t pwm_output) {
    SetPwmState(pwm_output, true);
}

void PwmDisable(uint8_t pwm_output) {
    SetPwmState(pwm_output, false);
}
