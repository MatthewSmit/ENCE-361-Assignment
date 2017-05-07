/**
 * Main Program
 */

#include "config.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"

#include "buttons.h"
#include "pwmManager.h"
#include "yawManager.h"
#include "serialInterface.h"
#include "heightMonitor.h"
#include "oledInterface.h"

#define DEBUG true

#define PWM_START_RATE_HZ       150
#define PWM_START_DC            50

#define PIN_BUFFER_SIZE 10

// Range the SysTick timer counts in
static uint32_t sysTickRange = 0;

// Current MS count
static volatile uint32_t timeCount = 0;

// Last read MS time for pin changes
static volatile uint32_t pinChangeLastMS = 0;

// Current ticks in read representing pin changes
static volatile uint32_t pinTicks[PIN_BUFFER_SIZE] = {};
static volatile uint32_t pinTickPtr = 0;

void SysTickHandler();
void PinChangeHandler();

void InitialiseClock() {
	// Sets the clock to 80 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}

void InitialiseSysTick() {
	// Sets the SysTick range to 1ms
	sysTickRange = SysCtlClockGet() / 1000;
    SysTickPeriodSet(sysTickRange);
    SysTickIntRegister(SysTickHandler);
    SysTickIntEnable();
    SysTickEnable();
}

void SysTickHandler() {
    timeCount++;

    updateHeight(); //triggers a height reading
}

void Draw(uint32_t frequency, uint32_t dutyCycle) {
    OLEDStringDraw("Milestone 1", 0, 0);

    char stringBuffer[30];
    memset(stringBuffer, 0, sizeof(stringBuffer));

    float yaw = getYawDegrees();
    sprintf(stringBuffer, "Yaw: %d  ", (int)yaw);
    OLEDStringDraw(stringBuffer, 0, 1);
    memset(stringBuffer, 0, sizeof(stringBuffer));

    uint32_t height = getHeightPercentage();
    sprintf(stringBuffer, "Height: %d  ", height);
    OLEDStringDraw(stringBuffer, 0, 2);
    memset(stringBuffer, 0, sizeof(stringBuffer));

    //sprintf(stringBuffer, "Freq: %d Hz  ", frequency);
    //OLEDStringDraw(stringBuffer, 0, 2);
    //memset(stringBuffer, 0, sizeof(stringBuffer));

    sprintf(stringBuffer, "Duty Cycle: %d%% ", dutyCycle);
    OLEDStringDraw(stringBuffer, 0, 3);
}

static bool lastButtonState[NUM_BUTTONS] = {};

bool IsButtonPressed(int button) {
    if (button < 0 || button >= NUM_BUTTONS)
        return false;

    return !lastButtonState[button] && checkButton(button);
}

void ButtonStateUpdate() {
    for (int i = 0; i < NUM_BUTTONS; i++)
        lastButtonState[i] = checkButton(i);
}

int main(void) {
    InitialiseClock();
    InitialiseSysTick();
    InitialisePWM(PWM_START_RATE_HZ, PWM_START_DC);
    //InitialisePin();

    initButtons();
    initSerial();
    initOLED();
    initYawManager();
    initHeightMonitor();

//    EnablePWM();

    IntMasterEnable();

    uint32_t frequency = PWM_START_RATE_HZ;
    uint32_t dutyCycle = PWM_START_DC;
	while (true) {
//	    uint32_t newFrequency = CalculateFrequency();
//
//		if (PinChangeTimedOut())
//			newFrequency = PWM_START_RATE_HZ;
//
//		if (newFrequency != frequency && newFrequency != 0)
//		{
//			// Clamp frequency to [100, 300] Hz
//			if (newFrequency < 100)
//				newFrequency = 100;
//
//			if (newFrequency > 300)
//				newFrequency = 300;
//
//			SetPWM(newFrequency, dutyCycle);
//			frequency = newFrequency;
//		}

        if (IsButtonPressed(BUT_DOWN))
        {
            dutyCycle -= 5;
            if (dutyCycle < 5)
                dutyCycle = 5;

            SetPWM(frequency, dutyCycle);
        }

        if (IsButtonPressed(BUT_UP))
        {
            dutyCycle += 5;
            if (dutyCycle > 95)
                dutyCycle = 95;

            SetPWM(frequency, dutyCycle);
        }

		ButtonStateUpdate();

		Draw(frequency, dutyCycle);
	}
}
