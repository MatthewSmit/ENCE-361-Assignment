#include "yawManager.h"

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"

#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"

#define NUMBER_SLOTS (112 * 4)

#define YAW_PERIPH_GPIO     SYSCTL_PERIPH_GPIOB
#define YAW_PERIPH_BASE     GPIO_PORTB_BASE
#define PIN_A               GPIO_PIN_0
#define PIN_B               GPIO_PIN_1
#define BOTH_PINS           (PIN_A | PIN_B)

typedef enum {
    STATE_LOW,
    STATE_A_HIGH,
    STATE_B_HIGH,
    STATE_BOTH_HIGH
} YawState;

static const int32_t states[] =
{
     0, +1, -1, 0,
     -1, 0, 0, +1,
     +1, 0, 0, -1,
     0, -1, +1, 0
};

static int32_t yaw;
static YawState oldState;

static void yawInterrupt() {
    YawState newState = (YawState)GPIOPinRead(YAW_PERIPH_BASE, BOTH_PINS);

    //Clear Interrupt
    GPIOIntClear(YAW_PERIPH_BASE, BOTH_PINS);

    int32_t change = states[newState | (oldState << 2)];
    yaw -= change;

    oldState = newState;
}

void initialiseYawManager() {
    SysCtlPeripheralEnable(YAW_PERIPH_GPIO);

    GPIOPinTypeGPIOInput(YAW_PERIPH_BASE, BOTH_PINS);
    GPIOPadConfigSet(YAW_PERIPH_BASE, BOTH_PINS, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    GPIODirModeSet(YAW_PERIPH_BASE, BOTH_PINS, GPIO_DIR_MODE_IN);
    GPIOIntTypeSet(YAW_PERIPH_BASE, BOTH_PINS, GPIO_BOTH_EDGES);
    GPIOIntRegister(YAW_PERIPH_BASE, yawInterrupt);
    GPIOIntEnable(YAW_PERIPH_BASE, BOTH_PINS);
    IntEnable(INT_GPIOB);
}

int32_t getYaw() {
    return yaw;
}

int32_t getYawDegrees() {
    int32_t tmpYaw = yaw;
    //tmpYaw %= NUMBER_SLOTS;
    int32_t value = (tmpYaw * 360) / NUMBER_SLOTS;
    //float value = (tmpYaw / (float)NUMBER_SLOTS) * 360;

    // Shift from [0, 360) to the range (-180, 180]
    /*if (value > 180)
        return value - 360;
    if (value <= -180)
        return value + 360;*/
    return value;
}
