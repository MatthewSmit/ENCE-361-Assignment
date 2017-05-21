/**
 * Main Program
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"
#include "utils/scheduler.h"

#include "buttons.h"
#include "height_manager.h"
#include "height_controller.h"
#include "oled_interface.h"
#include "pwm_output.h"
#include "serial_interface.h"
#include "yaw_controller.h"
#include "yaw_manager.h"

#define SYSTICK_FREQUENCY   200

tSchedulerTask g_psSchedulerTable[3];
uint32_t g_ui32SchedulerNumTasks = 3;

void Initialise();
void Draw();
void RegisterTasks();
void UpdateSerial();
void DemoButtons();

#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line) {
    while (1) {
    }
}
#endif

void Initialise() {
    SysCtlClockSet(
            SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN
                    | SYSCTL_XTAL_16MHZ);

    FPULazyStackingEnable();

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
    task_ptr->pfnFunction = UpdateButtons;
    task_ptr->ui32FrequencyTicks = 0;

    task_ptr++;
    task_ptr->bActive = true;
    task_ptr->pfnFunction = DemoButtons;
    task_ptr->ui32FrequencyTicks = 50;

//    task_ptr++;
//    task_ptr->bActive = true;
//    task_ptr->pfnFunction = Draw;
//    task_ptr->ui32FrequencyTicks = 10;

    task_ptr++;
    task_ptr->bActive = true;
    task_ptr->pfnFunction = UpdateSerial;
    task_ptr->ui32FrequencyTicks = 20;
}

void Draw() {
    char text_buffer[17];
    usnprintf(text_buffer, sizeof(text_buffer), "Ticks");
    OledStringDraw(text_buffer, 0, 0);
    usnprintf(text_buffer, sizeof(text_buffer), "%d", SchedulerTickCountGet());
    OledStringDraw(text_buffer, 0, 1);
}

void DemoButtons() {
    static double gain = 100.0;
    static double scale = 1.0;
    static bool started = false;
    uint8_t presses;
    presses = NumPushes(BTN_UP);
    gain += scale * presses;
    presses = NumPushes(BTN_DOWN);
    gain -= scale * presses;
    presses = NumPushes(BTN_RIGHT);
    if (presses > 0) {
        TuneParamTailRotor(0.0, 0.0, 0.0);
        SysCtlDelay(SysCtlClockGet() / 6);
        YawControllerInit();
        if (!started) {
            started = true;
            UARTprintf("start\n");
            SchedulerTaskEnable(2, true);
        }
    }
//        SchedulerTaskDisable(3);
    presses = NumPushes(BTN_LEFT);
    if (presses > 0) {
        if (started) {
            started = false;
            UARTprintf("end [%d]\n", (uint32_t) (gain * 1000));
            SchedulerTaskDisable(2);
            }
    }

//    TuneParamMainRotor(gain, 0.0, 0.0);
    TuneParamTailRotor(1.0, 0.0, gain / 1000.0);
}

void UpdateSerial() {
    UARTprintf("%d, %d\n", GetYaw(), SchedulerTickCountGet());
//    UARTprintf("Height: %d [%d]\n", GetHeight(), GetHeightPercentage());
//    UARTprintf("----------\n\n");
//	UARTprintf("k_p %d", (int) (k_p * 100));
#ifdef DEBUG

#endif
}

#ifdef DEBUG
void TimerHandler(void) {
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    UpdateHeight();
    UpdateYawController(1000 / PWM_FREQUENCY);
    SetPwmDutyCycle(MAIN_ROTOR, 30);
//    UpdateHeightController(1000 / PWM_FRE);
}

void TimerInit(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / PWM_FREQUENCY);

    TimerIntRegister(TIMER0_BASE, TIMER_A, TimerHandler);
    //
    // Setup the interrupts for the timer timeouts.
    //
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Enable the timers.
    //
    TimerEnable(TIMER0_BASE, TIMER_A);

}
#endif

int main() {
    Initialise();
    RegisterTasks();

#ifdef DEBUG
    SysCtlDelay(SysCtlClockGet());

    SetTargetHeight(20);
    TimerInit();
    PwmEnable(TAIL_ROTOR);
    PwmEnable(MAIN_ROTOR);
#endif

    IntMasterEnable();

    while (1) {
        SchedulerRun();
    }
}
