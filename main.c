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
#include "reset.h"
#include "serial_interface.h"
#include "yaw_controller.h"
#include "flight_controller.h"
#include "height.h"
#include "yaw.h"
#include "switch.h"

tSchedulerTask g_psSchedulerTable[6];
uint32_t g_ui32SchedulerNumTasks = 6;

/*
 * Register function prototypes.
 */
void Initialise(void);
void RegisterTasks(void);

/*
 * Register task function prototypes.
 */
void Draw();
void DemoButtons();
void UpdateSerial();

#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line) {
    while (1) {
    }
}
#endif

void Initialise(void) {
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

    SchedulerInit(PWM_FREQUENCY);
    SysTickIntRegister(SchedulerSysTickIntHandler);

    ResetInit();
    ButtonsInit();
    SwitchInit();

    YawManagerInit();
    HeightManagerInit();

    FlightControllerInit();

    OledInit();
    SerialInit();
}

void RegisterTasks(void) {
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
    task_ptr->ui32FrequencyTicks = 10; // Draw at 20 Hz.

    task_ptr++;
    task_ptr->bActive = true;
    task_ptr->pfnFunction = UpdateSerial;
    task_ptr->ui32FrequencyTicks = 50; // Update the UART at a rate of 4 Hz.
}

void Draw() {
    int32_t height = GetHeightPercentage();
    uint32_t target_height = GetTargetHeight();
    int32_t yaw = GetYawDegrees();
    int32_t target_yaw = GetTargetYawDegrees();
    char text_buffer[17];
    OledClearBuffer();
    usnprintf(text_buffer, sizeof(text_buffer), "Alt: %d [%d]", height,
            target_height);
    OledStringDraw(text_buffer, 0, 0);
    usnprintf(text_buffer, sizeof(text_buffer), "Yaw: %d [%d]", yaw,
            target_yaw);
    OledStringDraw(text_buffer, 0, 1);
}

void DemoButtons() {
//    static bool toggle = false;
//    uint8_t presses = NumPushes(BTN_UP);
//    if (presses > 0) {
//        toggle ? SchedulerTaskEnable(5, false) : SchedulerTaskDisable(5);
//    }
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

/**
 * Send heli info to UART.
 */
void UpdateSerial() {
    int32_t height = GetHeightPercentage();
    uint32_t target_height = GetTargetHeight();
    int32_t yaw = GetYawDegrees();
    int32_t target_yaw = GetTargetYawDegrees();
    uint32_t duty_cycle_main = GetPwmDutyCycle(MAIN_ROTOR);
    uint32_t duty_cycle_tail = GetPwmDutyCycle(TAIL_ROTOR);
    bool is_landed = GetFlightMode() == LANDED;
    char *flight_mode = is_landed ? "Landed" : "Flying";

    UARTprintf("Alt: %d [%d]\n"
            "Yaw: %d [%d]\n"
            "Main: [%d] Tail: [%d]\n"
            "Mode: %s\n"
            "\n", height, target_height, yaw, target_yaw, duty_cycle_main,
            duty_cycle_tail, flight_mode);
}

int main(void) {
    Initialise();
    RegisterTasks();
    IntMasterEnable();

    while (1) {
        SchedulerRun();
    }
}
