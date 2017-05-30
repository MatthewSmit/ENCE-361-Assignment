/**
 * @file yaw.c
 *
 * Handles the low level yaw detection.
 */

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"

#include "yaw.h"

#define YAW_PERIPH              SYSCTL_PERIPH_GPIOB
#define YAW_BASE                GPIO_PORTB_BASE
#define YAW_CHANNEL_A           GPIO_PIN_0
#define YAW_CHANNEL_B           GPIO_PIN_1
#define YAW_GPIO_PINS           (YAW_CHANNEL_A | YAW_CHANNEL_B)
#define YAW_INT                 INT_GPIOB

#define YAW_REF_PERIPH          SYSCTL_PERIPH_GPIOC
#define YAW_REF_BASE            GPIO_PORTC_BASE
#define YAW_REF_PIN             GPIO_PIN_4
#define YAW_REF_INT             INT_GPIOC

static volatile int32_t yaw = 0;
static volatile bool ref_found = false;

static const int8_t lookup_table[] = { 0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0 };

/**
 * Yaw interrupt handler.
 */
static void YawHandler(void) {
    static uint8_t state = 0;
    uint8_t previous_state = state;
    state = (uint8_t) GPIOPinRead(YAW_BASE, YAW_GPIO_PINS);

    GPIOIntClear(YAW_BASE, YAW_GPIO_PINS);

    /**
     * Increments the yaw depending on the state and the previous state.
     * Lookup table should never return 0, since this would indicate an invalid state transition.
     */
    yaw += lookup_table[state | (previous_state << 2)];
}

/**
 * Yaw reference interrupt handler.
 */
static void YawRefHandler(void) {
    /*
     * If the interrupt was caused by the reference signal.
     */
    if (GPIOIntStatus(YAW_REF_BASE, false) && YAW_REF_PIN) {
        GPIOIntDisable(YAW_REF_BASE, YAW_REF_PIN);
        GPIOIntClear(YAW_REF_BASE, YAW_REF_PIN);
        yaw = 0;
        ref_found = true;
    }
}

void YawDetectionInit(void) {
    /*
     * Initialise the yaw GPIO pins.
     */
    SysCtlPeripheralEnable(YAW_PERIPH);
    GPIOPinTypeGPIOInput(YAW_BASE, YAW_GPIO_PINS);
    GPIOPadConfigSet(YAW_BASE, YAW_GPIO_PINS, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    GPIODirModeSet(YAW_BASE, YAW_GPIO_PINS, GPIO_DIR_MODE_IN);

    /*
     * Initialise the yaw interrupts.
     */
    GPIOIntTypeSet(YAW_BASE, YAW_GPIO_PINS, GPIO_BOTH_EDGES);
    GPIOIntRegister(YAW_BASE, YawHandler);
    GPIOIntClear(YAW_BASE, YAW_GPIO_PINS);
    GPIOIntEnable(YAW_BASE, YAW_GPIO_PINS);
    IntEnable(YAW_INT);

    /*
     * Initialise the reference yaw GPIO pins.
     */
    SysCtlPeripheralEnable(YAW_REF_PERIPH);
    GPIOPinTypeGPIOInput(YAW_REF_BASE, YAW_REF_PIN);
    GPIOPadConfigSet(YAW_REF_BASE, YAW_REF_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIODirModeSet(YAW_REF_BASE, YAW_REF_PIN, GPIO_DIR_MODE_IN);

    /*
     * Initialise the reference yaw interrupt and disables it.
     */
    GPIOIntTypeSet(YAW_REF_BASE, YAW_REF_PIN, GPIO_RISING_EDGE);
    GPIOIntRegister(YAW_REF_BASE, YawRefHandler);
    GPIOIntClear(YAW_REF_BASE, YAW_REF_PIN);
    GPIOIntDisable(YAW_REF_BASE, YAW_REF_PIN);
    IntEnable(YAW_REF_INT);
}

void YawRefTrigger(void) {
    ref_found = false;
    GPIOIntEnable(YAW_REF_BASE, YAW_REF_PIN);
}

bool YawRefFound(void) {
    return ref_found;
}

int32_t GetYaw(void) {
    return yaw;
}

int32_t GetClosestYawRef(int32_t current_yaw) {
    /*
     * Gets the yaw remainder, in the range [0, YAW_FULL_ROTATION).
     */
    int32_t remainder = (current_yaw % YAW_FULL_ROTATION + YAW_FULL_ROTATION) % YAW_FULL_ROTATION;
    if (remainder >= YAW_FULL_ROTATION / 2)
        return current_yaw + YAW_FULL_ROTATION - remainder;
    else
        return current_yaw - remainder;
}

int32_t GetYawDegrees(void) {
    int32_t degrees = yaw * 360 / YAW_FULL_ROTATION;
    return degrees;
}
