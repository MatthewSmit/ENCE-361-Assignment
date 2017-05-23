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
#include "height_controller.h"
#include "oled_interface.h"
#include "serial_interface.h"
#include "yaw_controller.h"
#include "flight_controller.h"
#include "height.h"
#include "yaw.h"
#include "switch.h"

#define SYSTICK_FREQUENCY   200

tSchedulerTask g_psSchedulerTable[6];
uint32_t g_ui32SchedulerNumTasks = 6;

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
    /*
     * Set the clock to 80 MHz.
     */
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN| SYSCTL_XTAL_16MHZ);

    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    FPULazyStackingEnable();

    SchedulerInit(SYSTICK_FREQUENCY);
    SysTickIntRegister(SchedulerSysTickIntHandler);

    ButtonsInit();
    SwitchInit();

    YawManagerInit();
    HeightManagerInit();

    FlightControllerInit();

    //TODO
//    PwmEnable(MAIN_ROTOR);
//    PwmEnable(TAIL_ROTOR);
//    SetTargetHeight(50);

    OledInit();
    SerialInit();
}

void RegisterTasks() {
    tSchedulerTask *task_ptr = g_psSchedulerTable;
    task_ptr->bActive = true;
    task_ptr->pfnFunction = UpdateButtons;
    task_ptr->ui32FrequencyTicks = 2;

    task_ptr++;
    task_ptr->bActive = true;
    task_ptr->pfnFunction = UpdateSwitch;
    task_ptr->ui32FrequencyTicks = 2;

    task_ptr++;
    task_ptr->bActive = true;
    task_ptr->pfnFunction = UpdateFlightMode;
    task_ptr->ui32FrequencyTicks = 10;

    task_ptr++;
    task_ptr->bActive = false;
    task_ptr->pfnFunction = DemoButtons;
    task_ptr->ui32FrequencyTicks = 20;

    task_ptr++;
    task_ptr->bActive = true;
    task_ptr->pfnFunction = Draw;
    task_ptr->ui32FrequencyTicks = 10;

    task_ptr++;
    task_ptr->bActive = true;
    task_ptr->pfnFunction = UpdateSerial;
    task_ptr->ui32FrequencyTicks = 20;
}

void Draw() {
    char text_buffer[17];
    usnprintf(text_buffer, sizeof(text_buffer), "Ticks");
    OledStringDraw(text_buffer, 0, 0);
    usnprintf(text_buffer, sizeof(text_buffer), "%d", GetHeight());
    OledStringDraw(text_buffer, 0, 1);
}

void DemoButtons() {
    uint8_t presses = NumPushes(BTN_UP);
    if (presses > 0) {
        PwmDisable(MAIN_ROTOR);
        PwmDisable(TAIL_ROTOR);
    }
//    static double gain = 350.0;
//    static double scale = 50.0;
//    static bool started = false;
//    uint8_t presses;
//    presses = NumPushes(BTN_UP);
//    gain += scale * presses;
//    presses = NumPushes(BTN_DOWN);
//    gain -= scale * presses;
//    presses = NumPushes(BTN_RIGHT);
//    if (presses > 0) {
//        TuneParamMainRotor(0.0, 0.0, 0.0);
//        HeightControllerInit();
//        SysCtlDelay(SysCtlClockGet() / 3);
//        if (!started) {
//            started = true;
//            UARTprintf("start\n");
//            SchedulerTaskEnable(5, true);
//        }
//    }
////        SchedulerTaskDisable(3);
//    presses = NumPushes(BTN_LEFT);
//    if (presses > 0) {
//        if (started) {
//            started = false;
//            UARTprintf("end [%d]\n", (uint32_t) (gain));
//            SchedulerTaskDisable(5);
//        }
//    }
//
//    TuneParamMainRotor(0.0, gain, 0.0);
//    HeightControllerInit();
}

void UpdateSerial() {
    UARTprintf("Height: %3d, [%3d]\n", GetHeightPercentage(),
            GetTargetHeight());
    UARTprintf("Yaw:   %4d, [%4d]\n", GetYawDegrees(), GetTargetYaw());
//    UARTprintf("Height: %d [%d]\n", GetHeight(), GetHeightPercentage());
//    UARTprintf("----------\n\n");
//	UARTprintf("k_p %d", (int) (k_p * 100));
}

int main() {
    Initialise();
    RegisterTasks();
    IntMasterEnable();

    while (1) {
        SchedulerRun();
    }
}
