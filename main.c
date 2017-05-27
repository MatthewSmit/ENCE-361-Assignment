/**
 * Main Program
 */

#include <stdint.h>
#include <stdbool.h>

#include "driverlib/fpu.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "utils/scheduler.h"
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"

#include "buttons.h"
#include "flight_controller.h"
#include "height.h"
#include "height_controller.h"
#include "oled_interface.h"
#include "pwm_output.h"
#include "reset.h"
#include "serial_interface.h"
#include "switch.h"
#include "yaw.h"
#include "yaw_controller.h"

#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line) {
    while (1) {
    }
}
#endif

/*
 * Register task function prototypes.
 */
void Draw();
void UpdateSerial();

/*
 * Register function prototypes.
 */
void Initialise(void);

tSchedulerTask g_psSchedulerTable[5] = {
        [0] = { .bActive = true, .pfnFunction = UpdateButtons, .ui32FrequencyTicks = 2 },
        [1] = { .bActive = true, .pfnFunction = UpdateSwitch, .ui32FrequencyTicks = 2 },
        [2] = { .bActive = true, .pfnFunction = UpdateFlightMode, .ui32FrequencyTicks = 10 },
        [3] = { .bActive = true, .pfnFunction = UpdateSerial, .ui32FrequencyTicks = 50 },
        [4] = { .bActive = true, .pfnFunction = Draw, .ui32FrequencyTicks = 10 } };
uint32_t g_ui32SchedulerNumTasks = 5;

void Initialise(void) {
    /*
     * Set the clock to 80 MHz.
     */
    SysCtlClockSet(
    SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

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

void Draw() {
    int32_t height = GetHeightPercentage();
    uint32_t target_height = GetTargetHeight();
    int32_t yaw = GetYawDegrees();
    int32_t target_yaw = GetTargetYawDegrees();
    char text_buffer[17];
    OledClearBuffer();
    usnprintf(text_buffer, sizeof(text_buffer), "Alt: %d [%d]", height, target_height);
    OledStringDraw(text_buffer, 0, 0);
    usnprintf(text_buffer, sizeof(text_buffer), "Yaw: %d [%d]", yaw, target_yaw);
    OledStringDraw(text_buffer, 0, 1);
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
    const char *flight_mode = GetFlightMode();

    UARTprintf("Alt: %d [%d]\n"
            "Yaw: %d [%d]\n"
            "Main: [%d] Tail: [%d]\n"
            "Mode: %s\n"
            "\n", height, target_height, yaw, target_yaw, duty_cycle_main, duty_cycle_tail,
            flight_mode);
}

int main(void) {
    Initialise();
    IntMasterEnable();

    while (1) {
        SchedulerRun();
    }
}
