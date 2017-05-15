/**
 * Main Program
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"

#include "buttons.h"
#include "heightManager.h"
#include "yawManager.h"
#include "orbitOledInterface.h"
#include "serialInterface.h"
#include "pwmOutput.h"

#define DEBUG
#define SYSTICK_FREQUENCY 1000

static uint64_t time_counter = 0;

void Initialise();
void InitialiseSysTick();
void SysTickHandler();
void Draw();

void Initialise() {
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    InitialiseSysTick();
    InitialisePwm();
    InitialiseButtons();
    InitialiseSerial();
    InitialiseOled();
    InitialiseYawManager();
    InitialiseHeightMonitor();

    IntMasterEnable();
}

void InitialiseSysTick() {
	uint32_t systick_period = SysCtlClockGet() / SYSTICK_FREQUENCY;
    SysTickPeriodSet(systick_period);
    SysTickIntRegister(SysTickHandler);
    SysTickIntEnable();
    SysTickEnable();
}

void SysTickHandler() {
    time_counter++;
    UpdateButtons();
}

void Draw() {
    static uint8_t max_pushes = 0;
    static uint64_t last_systick = 0;
    uint32_t period = time_counter - last_systick;
    last_systick = time_counter;
    uint8_t tmp_pushes = NumPushes(BTN_UP);
    if (tmp_pushes > max_pushes) {
        max_pushes = tmp_pushes;
    }

    char string_buffer[30];
    usprintf(string_buffer, "Btn presses %d", max_pushes);
    OledStringDraw(string_buffer, 0, 1);
    usprintf(string_buffer, "FPS: %d", 1000 / period);
    OledStringDraw(string_buffer, 0, 2);
    SysCtlDelay(SysCtlClockGet() / (3 * 30));

}

int main() {
    Initialise();

	while (1) {
	    Draw();
	}
}
