#include "yawManager.h"

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"

#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"

#define NUMBER_SLOTS 112

#define YAW_PERIPH_GPIO     SYSCTL_PERIPH_GPIOB
#define YAW_PERIPH_BASE     GPIO_PORTB_BASE
#define PIN_A               GPIO_PIN_0
#define PIN_B               GPIO_PIN_1
#define BOTH_PINS           (PIN_A | PIN_B)

static int yaw;
static int state;

static void yawInterrupt() {

    int32_t pins = GPIOPinRead(YAW_PERIPH_BASE, BOTH_PINS);
    //Clear Interrupt
    GPIOIntClear(YAW_PERIPH_BASE, BOTH_PINS);

    if (state == BOTH_PINS) {
        if ((pins & PIN_B) == 0) {
            yaw++;
        }
        else {
            yaw--;
        }
    }

    state = pins;
}

void initYawManager() {
    SysCtlPeripheralEnable(YAW_PERIPH_GPIO);

    GPIOPinTypeGPIOInput(YAW_PERIPH_BASE, BOTH_PINS);
    GPIOPadConfigSet(YAW_PERIPH_BASE, BOTH_PINS, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    GPIODirModeSet(YAW_PERIPH_BASE, BOTH_PINS, GPIO_DIR_MODE_IN);
    GPIOIntTypeSet(YAW_PERIPH_BASE, BOTH_PINS, GPIO_BOTH_EDGES);
    GPIOIntRegister(YAW_PERIPH_BASE, yawInterrupt);
    GPIOIntEnable(YAW_PERIPH_BASE, BOTH_PINS);
    IntEnable(INT_GPIOB);
}

int getYaw() {
    return yaw;
}

float getYawDegrees() {
    int tmpYaw = yaw;
    tmpYaw %= NUMBER_SLOTS;
    float value = (tmpYaw / (float)NUMBER_SLOTS) * 360;

    // Shift from [0, 360) to the range (-180, 180]
    if (value > 180)
        return value - 360;
    if (value <= -180)
        return value + 360;
    return value;
}
