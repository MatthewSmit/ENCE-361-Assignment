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
#include "pwm.h"
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
void UpdateSerial();
void Tuning();

/*
 * Register function prototypes.
 */
void Initialise(void);

tSchedulerTask g_psSchedulerTable[5] = {
        [0] = { .bActive = true, .pfnFunction = UpdateButtons, .ui32FrequencyTicks = 2 },
        [1] = { .bActive = true, .pfnFunction = UpdateSwitch, .ui32FrequencyTicks = 2 },
        [2] = { .bActive = true, .pfnFunction = UpdateSerial, .ui32FrequencyTicks = 10 },
        [3] = { .bActive = true, .pfnFunction = Tuning, .ui32FrequencyTicks = 10 } };
uint32_t g_ui32SchedulerNumTasks = 5;

void Initialise(void) {
    /*
     * Set the clock to 80 MHz.
     */
    SysCtlClockSet(
    SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    /*
     * Enable lazy stacking for interrupt handlers. This allows floating-point
     * instructions to be used within interrupt handlers, but at the expense of
     * extra stack usage.
     */
    FPULazyStackingEnable();

    SchedulerInit(PWM_FREQUENCY);
    SysTickIntRegister(SchedulerSysTickIntHandler);

    ResetInit();
    ButtonsInit();
    SwitchInit();

    YawManagerInit();
    HeightManagerInit();

    PwmInit();
    HeightControllerInit();
    YawControllerInit();

    TuneProportionalMainRotor(0.0);

    PriorityTaskInit();
    PriorityTaskEnable();

    SerialInit();
    SchedulerTaskDisable(2);

    SetTargetYawDegrees(0);
    SetTargetHeight(30);
    ZeroHeightTrigger();

    PwmEnable(MAIN_ROTOR);
    PwmEnable(TAIL_ROTOR);
}

void Tuning() {
	static double inc = 1.0;
	static double gain = 0.0;
	uint8_t presses[NUM_BUTTONS];
	static bool started = false;

	if (!started) {
		for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
			presses[i] = NumPushes(i);
		}

		if (presses[BTN_UP] > 0) {
			gain += inc * presses[BTN_UP];
			UARTprintf("gain [%4d]\n", (uint32_t) (gain * 1000));
		}
		if (presses[BTN_DOWN] > 0) {
			gain -= inc * presses[BTN_DOWN];
			UARTprintf("gain [%4d]\n", (uint32_t) (gain * 1000));
		}

		if (presses[BTN_LEFT] > 0) {
			inc /= 0.1;
		}

		if (presses[BTN_RIGHT] > 0) {
			inc *= 0.1;
		}

		/*
		 * Don't let the gain fail below zero
		 */
		gain = gain < 0 ? 0 : gain;
	}

    if (GetSwitchEvent() == SWITCH_UP) {
    	if (!started) {
    		started = true;
            TuneProportionalMainRotor(gain);
            SysCtlDelay(SysCtlClockGet() / 6);
    		UARTprintf("start\n");
    		SchedulerTaskEnable(2, true);
    	}
    } else {
    	if (started) {
    		started = false;
            UARTprintf("end [%4d]\n", (uint32_t) (gain * 1000));
            SchedulerTaskDisable(2);
    	}
    }
}

/**
 * Send heli info to UART.
 */
void UpdateSerial() {
    int32_t height = GetHeightPercentage();
    int32_t time = SchedulerTickCountGet() * 1000 / PWM_FREQUENCY;
    UARTprintf("%d, %d\n", height, GetPwmDutyCycle(MAIN_ROTOR));
}

int main(void) {
    Initialise();
    IntMasterEnable();

    while (1) {
        SchedulerRun();
    }
}
