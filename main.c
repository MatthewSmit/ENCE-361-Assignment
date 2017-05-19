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

#define SYSTICK_FREQUENCY   200
#define SYSTICK_PERIOD_MS   5

struct Task {
    uint32_t period;
    uint32_t elapsed_ticks;
    void (*TaskCallback)(void);
};

static const uint8_t num_tasks = 2;
static struct Task tasks[2];

static volatile uint64_t scheduler_ticks;

void Initialise();
void SysTickInit();
void SysTickHandler();
void Draw();
void RegisterTasks();
void UpdateSerial();

void Initialise() {
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysTickInit();
    PwmInit();
    ButtonsInit();
    YawManagerInit();
    HeightManagerInit();

    OledInit();
    SerialInit();
}

void SysTickInit() {
    SysTickPeriodSet(SysCtlClockGet() / SYSTICK_FREQUENCY);
    SysTickIntRegister(SysTickHandler);
    SysTickIntEnable();
    SysTickEnable();
}

void SysTickHandler() {
    scheduler_ticks++;
    for (uint8_t i = 0; i < num_tasks; i++) {
        if (tasks[i].elapsed_ticks >= tasks[i].period) {
            tasks[i].elapsed_ticks = 0;
            tasks[i].TaskCallback();
        }
        tasks[i].elapsed_ticks++;
    }
}

void RegisterTasks() {
    uint8_t i = 0;
    tasks[i].period = 1;
    tasks[i].elapsed_ticks = tasks[i].period;
    tasks[i].TaskCallback = UpdateHeight;
    i++;
    tasks[i].period = 1;
    tasks[i].elapsed_ticks = tasks[i].period;
    tasks[i].TaskCallback = UpdateButtons;
}

void Draw() {
    char text_buffer[17];
    usnprintf(text_buffer, sizeof(text_buffer), "Ticks");
    OledStringDraw(text_buffer, 0, 0);
    usnprintf(text_buffer, sizeof(text_buffer), "%d", (uint32_t) scheduler_ticks);
    OledStringDraw(text_buffer, 0, 1);
}

void UpdateSerial() {
    UARTprintf("Height: %d\n", GetHeight());
    UARTprintf("Height %%: %d\n", GetHeightPercentage());
}

int main() {
    RegisterTasks();
    Initialise();
    IntMasterEnable();

	while (1) {
	    if (scheduler_ticks % 10 == 0)
	        Draw();
	    if (scheduler_ticks % 50 == 0)
	        UpdateSerial();
	    SysCtlSleep();
	}
}
