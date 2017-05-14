#include "pwmManager.h"

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

/* PWM Definitions */
#define PWM_MAIN_BASE           PWM0_BASE
#define PWM_MAIN_GEN            PWM_GEN_3
#define PWM_MAIN_OUTNUM         PWM_OUT_7
#define PWM_MAIN_OUTBIT         PWM_OUT_7_BIT
#define PWM_MAIN_PERIPH_PWM     SYSCTL_PERIPH_PWM0
#define PWM_MAIN_PERIPH_GPIO    SYSCTL_PERIPH_GPIOC
#define PWM_MAIN_GPIO_BASE      GPIO_PORTC_BASE
#define PWM_MAIN_GPIO_CONFIG    GPIO_PC5_M0PWM7
#define PWM_MAIN_GPIO_PIN       GPIO_PIN_5

#define PWM_DIVIDER_CODE        SYSCTL_PWMDIV_16
#define PWM_DIVIDER             16

void InitialisePWM(int frequency, int dutyCycle) {
    SysCtlPWMClockSet(PWM_DIVIDER_CODE);

    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);

    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);

    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    SetPWM(frequency, dutyCycle);

    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);

    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);
}

void SetPWM(int frequency, int dutyCycle) {
    uint32_t period = SysCtlClockGet() / PWM_DIVIDER / frequency;

    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, period);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM, period * dutyCycle / 100);
}

void EnablePWM() {
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);
}
