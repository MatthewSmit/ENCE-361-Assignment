#include "config.h"

#include <stdio.h>

#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

#include "OrbitOLEDInterface.h"

#define SYSTICK_RATE_HZ  1000

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

#define PWM_START_RATE_HZ       150
#define PWM_START_DC            50

#define PIN_BUFFER_SIZE 10

static volatile uint32_t timeCount = 0;

static volatile uint32_t pinTicks[PIN_BUFFER_SIZE] = {};
static volatile uint32_t pinTickPtr = 0;

void SysTickHandler();
void PinChangeHandler();

void SetPWM(int frequency, int dutyCycle) {
    uint32_t period = SysCtlClockGet() / PWM_DIVIDER / frequency;

    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, period);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM, period * dutyCycle / 100);
}

void InitialiseClock() {
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPWMClockSet(PWM_DIVIDER_CODE);
}

void InitialisePWM() {
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWM);
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);

    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);

    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    SetPWM(PWM_START_RATE_HZ, PWM_START_DC);

    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);

    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);
}

void InitialisePin() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOIntRegister(GPIO_PORTB_BASE, PinChangeHandler);

    GPIOPinTypeGPIOInput (GPIO_PORTB_BASE, GPIO_PIN_0);
    GPIOPadConfigSet (GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_RISING_EDGE);

    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0);
    IntEnable(INT_GPIOB);
}

void InitialiseSysTick() {
    SysTickPeriodSet(SysCtlClockGet() / SYSTICK_RATE_HZ);
    SysTickIntRegister(SysTickHandler);
    SysTickIntEnable();
    SysTickEnable();
}

void SysTickHandler() {
    timeCount++;
}

void PinChangeHandler() {
    static uint32_t lastCount = 0;

    //Clear Interrupt
    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0);

    uint32_t currentCount = timeCount;
    uint32_t difference = currentCount - lastCount;
    lastCount = currentCount;

    pinTicks[pinTickPtr] = difference;
    pinTickPtr = (pinTickPtr + 1) % PIN_BUFFER_SIZE;
}

uint32_t CalculateFrequency() {
    uint32_t sum = 0;
    for (int i = 0; i < PIN_BUFFER_SIZE; i++)
        sum += pinTicks[i];
    sum /= PIN_BUFFER_SIZE;
    return 1000 / sum;
}

void Draw(uint32_t frequency, uint32_t dutyCycle) {
    OLEDStringDraw("Milestone 1", 0, 0);

    char stringBuffer[20];
    sprintf(stringBuffer, "Freq: %d Hz", frequency);
    OLEDStringDraw(stringBuffer, 0, 2);

    sprintf(stringBuffer, "Duty Cycle: %d%%", dutyCycle);
    OLEDStringDraw(stringBuffer, 0, 3);
}

int main(void) {
    InitialiseClock();
    InitialisePWM();
    InitialisePin();
    InitialiseSysTick();
    OLEDInitialise();

    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);

    IntMasterEnable();

    uint32_t frequency = PWM_START_RATE_HZ;
    uint32_t dutyCycle = PWM_START_DC;
	
	while (true) {
		uint32_t newFrequency = CalculateFrequency();

		if (newFrequency != frequency && newFrequency != 0) {
			SetPWM(newFrequency, dutyCycle);
			frequency = newFrequency;
		}

		Draw(frequency, dutyCycle);
	}
}
