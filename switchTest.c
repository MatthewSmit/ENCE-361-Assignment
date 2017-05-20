/*
 * switchTest.c
 *
 *  Created on: 2017-05-20
 *      Author: Matthew Smit
 */

#include <stdbool.h>
#include <stdint.h>

#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "utils/ustdlib.h"

#include "orbitOledInterface.h"
#include "switch.h"

#define SYSTICK_FREQUENCY   200

struct Task {
    uint32_t period;
    uint32_t elapsed_ticks;
    void (*TaskCallback)(void);
};

static const uint8_t num_tasks = 1;
static struct Task tasks[1];

static volatile uint64_t scheduler_ticks;

void SysTickInit();
void SysTickHandler();
void Draw();
void RegisterTasks();

void Initialise() {
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysTickInit();
    SwitchInit();

    OledInit();
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
    tasks[i].TaskCallback = SwitchUpdate;
}

void Draw() {
    uint8_t status = GetSwitchState();

    char text_buffer[17];
    usnprintf(text_buffer, sizeof(text_buffer), "Switch State:");
    OledStringDraw(text_buffer, 0, 0);
    usnprintf(text_buffer, sizeof(text_buffer), "%s", status == DOWN ? "down" : "up  ");
    OledStringDraw(text_buffer, 0, 1);
}

int main() {
    RegisterTasks();
    Initialise();
    IntMasterEnable();

    while (1) {
        Draw();
    }
}
