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
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "utils/scheduler.h"

#include "buttons.h"
#include "pwm_output.h"
#include "yaw_controller.h"
#include "height_controller.h"
#include "switch.h"
#include "buttons.h"
#include "yaw.h"
#include "height.h"
#include "flight_controller.h"

void TimerInit(void);
void TimerHandler(void);
void PriorityTaskDisable(void);
void PriorityTaskEnable(void);
void UpdateError(void);
void ResetError(void);
bool IsYawErrorTolerance(void);
bool IsHeightErrorTolerance(void);

#define TIMER_PERIPH			SYSCTL_PERIPH_TIMER0
#define TIMER_BASE				TIMER0_BASE
#define TIMER_CONFIG			TIMER_CFG_PERIODIC
#define TIMER_TIMER				TIMER_A
#define TIMER_TIMEOUT			TIMER_TIMA_TIMEOUT
#define TIMER_INT				INT_TIMER0A

/*
 * Rate of descent (ms per decrement of duty cycle)
 */
#define RATE_OF_DESCENT			    35
/*
 * Acceptable tolerance for yaw error (rotation unit defined in YAW_FULL_ROTATION)
 */
#define YAW_SAMPLE_TOLERANCE        2
/*
 * Acceptable tolerance for height error (%)
 */
#define HEIGHT_SAMPLE_TOLERANCE     1
/*
 * Number of samples to summate error over
 */
#define NUM_ERROR_SAMPLES             5

static const uint8_t height_inc = 10;
static const uint8_t height_min = 0;
static const uint8_t height_max = 100;
static const uint8_t yaw_inc = 15;

/*
 * Tolerance to ascertain if yaw has reached target yaw. Rounded up.
 */
static uint16_t const yaw_tolerance = (uint16_t) (YAW_SAMPLE_TOLERANCE * NUM_ERROR_SAMPLES);
static uint16_t yaw_error_buf[NUM_ERROR_SAMPLES];

/*
 * Tolerance to ascertain if height has reached target height. Rounded up.
 */
static uint16_t const height_tolerance = (uint16_t) (HEIGHT_SAMPLE_TOLERANCE * NUM_ERROR_SAMPLES);
static uint16_t height_error_buf[NUM_ERROR_SAMPLES];

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

void UpdateError(void) {
    static uint32_t idx = 0;
    uint16_t yaw_sample_err = (uint16_t) abs(GetYaw() - GetTargetYaw());
    uint16_t height_sample_err = (uint16_t) abs(GetHeight() - (int32_t) GetTargetHeight());
    yaw_error_buf[idx] = yaw_sample_err;
    height_error_buf[idx] = height_sample_err;
    idx = (idx + 1) % NUM_ERROR_SAMPLES;
}

void ResetError(void) {
    for (uint8_t i = 0; i < NUM_ERROR_SAMPLES; i++) {
        yaw_error_buf[i] = UINT16_MAX;
        height_error_buf[i] = UINT16_MAX;
    }
}

bool IsYawErrorTolerance(void) {
    uint16_t err_sum = 0;
    for (uint8_t i = 0; i < NUM_ERROR_SAMPLES; i++) {
        err_sum += yaw_error_buf[i];
    }
    return !(err_sum > yaw_tolerance);
}

bool IsHeightErrorTolerance(void) {
    uint16_t err_sum = 0;
    for (uint8_t i = 0; i < NUM_ERROR_SAMPLES; i++) {
        err_sum += height_error_buf[i];
    }
    return !(err_sum > height_tolerance);
}

void UpdateFlightMode() {
    static bool wait = false;
    static bool wait_2 = false;
    bool event = GetSwitchEvent();
    uint8_t presses[4];
	switch (flight_state) {

    case LANDED: {
        if (event == SWITCH_UP) {
            //
            // Go to INIT state
            //
            flight_state = INIT;
        }
        break;
    }
    case INIT: {
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
//            SetPwmDutyCycle(TAIL_ROTOR, 2);
            SetPwmDutyCycle(MAIN_ROTOR, 30);
//            PwmEnable(TAIL_ROTOR);
            PwmEnable(MAIN_ROTOR);
        }
        break;
    }
    case FLYING: {
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
                target_height = GetTargetHeight() + presses[BTN_UP] * height_inc;
                target_height = (target_height > height_max) ? height_max : target_height;
                SetTargetHeight(target_height);
            }

            if (presses[BTN_DOWN] > 0) {
                //
                // Decrease height
                //
                target_height = GetTargetHeight() - presses[BTN_DOWN] * height_inc;
                target_height = (target_height < height_min) ? height_min : target_height;
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
    }
    case LANDING: {
        UpdateError();
        uint32_t elapsed_ticks = 0;
        bool is_yaw_target_reached = IsYawErrorTolerance();
        bool is_height_target_reached = IsHeightErrorTolerance();
        if (!wait) {
            //
            // Wait until yaw is at closest reference.
            //
            wait = true;
            int32_t yaw_ref = GetClosestYawRef(target_yaw);
            SetTargetYaw(yaw_ref);

            /*
             * Reset the error mechanism used to detect if target yaw and height have been reached.
             */
            ResetError();
        } else {
            if (wait_2) {
                if (GetTargetHeight() > 0) {
                    if ((SchedulerElapsedTicksGet(elapsed_ticks) * (1000 / PWM_FREQUENCY))
                            >= RATE_OF_DESCENT) {
                        elapsed_ticks = SchedulerTickCountGet();
                        SetTargetHeight(GetTargetHeight() - 1);
                    }
                }
                if (is_yaw_target_reached && is_height_target_reached) {
                    wait = false;
                    wait_2 = false;
                    PwmDisable(MAIN_ROTOR);
                    PwmDisable(TAIL_ROTOR);
                    flight_state = LANDED;
                }
            } else if (is_yaw_target_reached) {
                /*
                 * Wait until all landing criteria are met.
                 */
                wait_2 = true;
                elapsed_ticks = SchedulerTickCountGet();
            }
        }
        break;
    }
	}
}

uint8_t GetFlightMode(void) {
    return flight_state;
}
