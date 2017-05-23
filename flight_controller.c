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
#include "yaw_manager.h"
#include "height_controller.h"
#include "switch.h"

#define TIMER_PERIPH			SYSCTL_PERIPH_TIMER0
#define TIMER_BASE				TIMER0_BASE
#define TIMER_CONFIG			TIMER_CFG_PERIODIC
#define TIMER_TIMER				TIMER_A
#define TIMER_TIMEOUT			TIMER_TIMA_TIMEOUT
#define TIMER_INT				INT_TIMER0A

#define HEIGHT_STEP             10
#define YAW_STEP                15

enum {LANDED, INIT, FLYING, LANDING};

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
     * Enable the timers.
     */
    TimerEnable(TIMER_BASE, TIMER_TIMER);

    /*
     * Trigger ADC to capture height.
     */
    TimerADCEventSet(TIMER_BASE, TIMER_ADC_TIMEOUT_A);
}

void PriorityTaskInit(void) {
	TimerInit();
}

void InitFlight(void) {
	flight_state = LANDED;
	target_yaw = 0;
	target_height = 0;
	SysCtlDelay(SysCtlClockGet());

	SetTargetHeight(20);
	PwmEnable(TAIL_ROTOR);
	PwmEnable(MAIN_ROTOR);
}

void UpdateFlying(void) {
    for (int32_t pushes = NumPushes(BTN_UP); pushes > 0; pushes--) {
        target_height += HEIGHT_STEP;
        if (target_height > 100) {
            target_height = 100;
        }
    }

    for (int32_t pushes = NumPushes(BTN_DOWN); pushes > 0; pushes--) {
        target_height -= HEIGHT_STEP;
        if (target_height < 0) {
            target_height = 0;
        }
    }

    for (int32_t pushes = NumPushes(BTN_LEFT); pushes > 0; pushes--) {
        target_yaw -= YAW_STEP;
        if (target_yaw < -360) {
            target_yaw += 720;
        }
    }

    for (int32_t pushes = NumPushes(BTN_RIGHT); pushes > 0; pushes--) {
        target_yaw += YAW_STEP;
        if (target_yaw > 360) {
            target_yaw -= 720;
        }
    }

    SetTargetHeight(target_height);
    SetTargetYaw(target_yaw);
}

bool HasLanded(void) {
    //TODO
    return true;
}

void UpdateFlightMode(void) {
	bool event = GetSwitchEvent();
	switch (flight_state) {
	case LANDED:
		if (event == UP) {
		    flight_state = INIT;
		    //TODO: Start Init task to find reference yaw
		}
		break;
	case INIT:
	    if (YawRefFound()) {
	        flight_state = FLYING;
            ClearButtons();
            SetTargetYaw(0);
	    }
		break;
	case FLYING:
	    if (event == DOWN) {
	        flight_state = LANDING;
	        SetTargetHeight(0);
	        SetTargetYaw(0);
	    }
	    else {
	        UpdateFlying();
	    }
		break;
	case LANDING:
	    if (HasLanded()) {
	        flight_state = LANDED;
	    }
		break;
	}
}
