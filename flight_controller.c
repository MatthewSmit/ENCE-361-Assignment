/*
 * flight_controller.c
 *
 *  Created on: May 22, 2017
 *      Author: dpv11
 */
#include "stdint.h"
#include "stdbool.h"

#include "inc/hw_ints.h"
#include "inc/hw_timer.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

#include "buttons.h"
#include "pwm_output.h"
#include "yaw_controller.h"
#include "height_controller.h"
#include "switch.h"
#include "buttons.h"
#include "yaw.h"
#include "height.h"
#include "flight_controller.h"

#define TIMER_PERIPH			SYSCTL_PERIPH_TIMER0
#define TIMER_BASE				TIMER0_BASE
#define TIMER_CONFIG			TIMER_CFG_PERIODIC
#define TIMER_TIMER				TIMER_A
#define TIMER_TIMEOUT			TIMER_TIMA_TIMEOUT
#define TIMER_INT				INT_TIMER0A

static const uint8_t height_inc = 10;
static const uint8_t height_min = 0;
static const uint8_t height_max = 100;
static const uint8_t yaw_inc = 15;

static uint8_t flight_state;
static int32_t target_yaw;
static int32_t target_height;

void TimerHandler(void) {
    TimerIntClear(TIMER_BASE, TIMER_TIMEOUT);
    UpdateYawController(1000 / PWM_FREQUENCY);
    UpdateHeightController(1000 / PWM_FREQUENCY);
}

void TimerInit(void) {
    SysCtlPeripheralEnable(TIMER_PERIPH);
    TimerConfigure(TIMER_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER_BASE, TIMER_TIMER, SysCtlClockGet() / PWM_FREQUENCY);

    TimerIntRegister(TIMER_BASE, TIMER_TIMER, TimerHandler);

    /*
     * Setup the interrupts for the timer timeouts.
     */
    IntEnable(TIMER_INT);
    TimerIntEnable(TIMER_BASE, TIMER_TIMEOUT);

    /*
     * Trigger ADC to capture height.
     */
    TimerADCEventSet(TIMER_BASE, TIMER_TIMEOUT);
    TimerControlTrigger(TIMER_BASE, TIMER_TIMER, true);

    /*
     * Enable the timers.
     */
    TimerEnable(TIMER_BASE, TIMER_TIMER);
}

void PriorityTaskInit(void) {
    TimerInit();
}

void PriorityTaskDisable(void) {
    TimerIntDisable(TIMER_BASE, TIMER_TIMEOUT);
}

void PriorityTaskEnable(void) {
    TimerIntEnable(TIMER_BASE, TIMER_TIMEOUT);
}

void FlightControllerInit(void) {
	flight_state = LANDED;
    PwmInit();
    SetTargetHeight(0);
    SetTargetYawDegrees(0);
    YawControllerInit();
    HeightControllerInit();
    PriorityTaskInit();
}

void UpdateFlightMode() {
    static bool wait = false;
    static bool wait_2 = false;
    bool event = GetSwitchEvent();
    uint8_t presses[4];
	switch (flight_state) {

	case LANDED:
        if (event == SWITCH_UP) {
            //
            // Go to INIT state
            //
            flight_state = INIT;
        }
		break;

	case INIT:
        //
        // Ignore all switch events
        //
        if (YawRefFound()) {
            wait = false;
            //
            // Go straight to FLYING state.
            //
            YawControllerInit();
            HeightControllerInit();
            PwmEnable(MAIN_ROTOR);
            PwmEnable(TAIL_ROTOR);
            PriorityTaskEnable();
            ResetPushes();
            flight_state = FLYING;
        } else if (!wait) {
            wait = true;
            YawRefTrigger();
            ZeroHeightTrigger();
            PriorityTaskDisable();
            SetPwmDutyCycle(TAIL_ROTOR, 2);
            SetPwmDutyCycle(MAIN_ROTOR, 20);
            PwmEnable(TAIL_ROTOR);
            PwmEnable(MAIN_ROTOR);
        }
        break;

	case FLYING:
        if (event == SWITCH_DOWN) {
            //
            // Go to LANDING state
            //
            flight_state = LANDING;
        } else {
            //
            // Get all the button pushes.
            //
            for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
                presses[i] = NumPushes(i);
            }

            if (presses[BTN_UP] > 0) {
                //
                // Increase height
                //
                target_height = GetTargetHeight()
                        + presses[BTN_UP] * height_inc;
                target_height =
                        (target_height > height_max) ?
                                height_max : target_height;
                SetTargetHeight(target_height);
            }

            if (presses[BTN_DOWN] > 0) {
                //
                // Decrease height
                //
                target_height = GetTargetHeight()
                        - presses[BTN_DOWN] * height_inc;
                target_height =
                        (target_height < height_min) ?
                                height_min : target_height;
                SetTargetHeight(target_height);
            }

            if (presses[BTN_LEFT] > 0) {
                //
                // Rotate counter-clockwise
                //
                target_yaw = GetTargetYawDegrees() - presses[BTN_LEFT] * yaw_inc;
                SetTargetYawDegrees(target_yaw);
            }

            if (presses[BTN_RIGHT] > 0) {
                //
                // Rotate clockwise
                //
                target_yaw = GetTargetYawDegrees() + presses[BTN_RIGHT] * yaw_inc;
                SetTargetYawDegrees(target_yaw);
            }
        }
		break;

	case LANDING:
        target_yaw = GetTargetYaw();
        if (!wait) {
            //
            // Wait until yaw is at closest reference.
            //
            wait = true;
            int32_t yaw_ref = GetClosestYawRef(target_yaw);
            SetTargetYaw(yaw_ref);
        } else if (wait_2) {
            if ((GetYaw() == target_yaw)
                    && (GetHeight() == GetTargetHeight())) {
                wait = false;
                wait_2 = false;
                PwmDisable(MAIN_ROTOR);
                PwmDisable(TAIL_ROTOR);
                // TODO Reset the yaw to zero.
                flight_state = LANDED;
            }
        } else if (!wait_2 && GetYaw() == target_yaw) {
            //
            // Wait until all landing criteria are met.
            //
            wait_2 = true;
            SetTargetHeight(height_min);
        }
		break;
	}
}

uint8_t GetFlightMode(void) {
    return flight_state;
}
