/**
 * Main Program
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/adc.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"
#include "utils/scheduler.h"
#include "buttons.h"
#include "heightManager.h"
#include "yawManager.h"
#include "orbitOledInterface.h"
#include "serialInterface.h"
#include "pwmOutput.h"

#define SYSTICK_FREQUENCY   200

tSchedulerTask g_psSchedulerTable[3];
uint32_t g_ui32SchedulerNumTasks = 3;

void Initialise();
void Draw();
void RegisterTasks();
void UpdateSerial();

#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line) {
    while (1) {
    }
}
#endif

void Initialise() {
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SchedulerInit(SYSTICK_FREQUENCY);
    SysTickIntRegister(SchedulerSysTickIntHandler);

    PwmInit();
    ButtonsInit();
    YawManagerInit();
    HeightManagerInit();

    OledInit();
    SerialInit();
}

void RegisterTasks() {
    tSchedulerTask *task_ptr = g_psSchedulerTable;
    task_ptr->bActive = true;
    task_ptr->pfnFunction = UpdateSerial;
    task_ptr->ui32FrequencyTicks = 0;

    task_ptr++;
    task_ptr->bActive = true;
    task_ptr->pfnFunction = Draw;
    task_ptr->ui32FrequencyTicks = 10;

    task_ptr++;
    task_ptr->bActive = true;
    task_ptr->pfnFunction = UpdateSerial;
    task_ptr->ui32FrequencyTicks = 50;
}

void Draw() {
    char text_buffer[17];
    usnprintf(text_buffer, sizeof(text_buffer), "Ticks");
    OledStringDraw(text_buffer, 0, 0);
    usnprintf(text_buffer, sizeof(text_buffer), "%d", SchedulerTickCountGet());
    OledStringDraw(text_buffer, 0, 1);
}

void UpdateSerial() {
    UARTprintf("Height: %d\n", GetHeight());
    UARTprintf("Height %%: %d\n", GetHeightPercentage());
#ifdef DEBUG
    UARTprintf("Debug only");
#endif
}

int main() {
    Initialise();
    RegisterTasks();

    IntMasterEnable();

	while (1) {
	    SchedulerRun();
	}
}
