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

#define SYSTICK_FREQUENCY   1000
#define SYSTICK_PERIOD_MS   1

struct Task {
    /* Task Period (ms) */
    uint32_t period;
    uint32_t elapsed_time;
    void (*TaskCallback)(void);
};

static const uint8_t num_tasks = 2;
static struct Task tasks[2];

static volatile uint64_t sys_clock = 0;

void Initialise();
void InitialiseSysTick();
void SysTickHandler();
void Draw();
void RegisterTasks();

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
    sys_clock++;
    for (uint8_t i = 0; i < num_tasks; i++) {
        if (tasks[i].elapsed_time >= tasks[i].period) {
            tasks[i].TaskCallback();
            tasks[i].elapsed_time = 0;
        }
        tasks[i].elapsed_time += SYSTICK_PERIOD_MS;
    }
}

void RegisterTasks() {
    uint8_t i = 0;
    tasks[i].period = 5;
    tasks[i].elapsed_time = tasks[i].period;
    tasks[i].TaskCallback = UpdateButtons;
    i++;
    tasks[i].period = 5;
    tasks[i].elapsed_time = tasks[i].period;
    tasks[i].TaskCallback = UpdateHeight;
//    i++;
//    tasks[i].period = 50;
//    tasks[i].elapsed_time = tasks[i].period;
//    tasks[i].TaskCallback = Draw;
}

void Draw() {
    char text_buffer[17];
    usnprintf(text_buffer, sizeof(text_buffer), "Hello World%d", 1);
    OledStringDraw(text_buffer, 0, 1);
//    static uint8_t max_pushes = 0;
//    static uint64_t tmp_clock = 0;
//    uint32_t period = sys_clock - tmp_clock;
//    tmp_clock = sys_clock;
//    uint8_t tmp_pushes = NumPushes(BTN_DOWN);
//    if (tmp_pushes > max_pushes) {
//        max_pushes = tmp_pushes;
//    }
//
//    char text_buffer[17];
//    uvsnprintf(text_buffer, "Btn presses %2d", max_pushes);
//    OledStringDraw(text_buffer, 0, 1);
//    usprintf(text_buffer, "FPS: %3d  ", period * SYSTICK_PERIOD_MS);
//    OledStringDraw(text_buffer, 0, 2);
//    SysCtlDelay(SysCtlClockGet() / (3 * 30));
//    SysCtlSleep();
}

int main() {
    RegisterTasks();
    Initialise();

	while (1) {
	    Draw();
	}
}
