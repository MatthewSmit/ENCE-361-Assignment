#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"

#define NUMBER_SLOTS 112
#define YAW_FULL_ROTATION   (NUMBER_SLOTS * 4)

#define YAW_PERIPH_GPIO     SYSCTL_PERIPH_GPIOB
#define YAW_PERIPH_BASE     GPIO_PORTB_BASE
#define YAW_CHANNEL_A       GPIO_PIN_0
#define YAW_CHANNEL_B       GPIO_PIN_1
#define YAW_GPIO_PINS       (YAW_CHANNEL_A | YAW_CHANNEL_B)
#define YAW_INT             INT_GPIOB

static int32_t yaw;

static const int8_t lookup_table[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

void YawHandler() {
    static uint8_t state = 0;
    uint8_t previous_state = state;
    state = (uint8_t) GPIOPinRead(YAW_PERIPH_BASE, YAW_GPIO_PINS);

    GPIOIntClear(YAW_PERIPH_BASE, YAW_GPIO_PINS);

    yaw += lookup_table[state | (previous_state << 2)];
}

void InitialiseYawManager() {
    SysCtlPeripheralEnable(YAW_PERIPH_GPIO);

    GPIOPinTypeGPIOInput(YAW_PERIPH_BASE, YAW_GPIO_PINS);
    GPIOPadConfigSet(YAW_PERIPH_BASE, YAW_GPIO_PINS, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    GPIODirModeSet(YAW_PERIPH_BASE, YAW_GPIO_PINS, GPIO_DIR_MODE_IN);
    GPIOIntTypeSet(YAW_PERIPH_BASE, YAW_GPIO_PINS, GPIO_BOTH_EDGES);
    GPIOIntRegister(YAW_PERIPH_BASE, YawHandler);
    GPIOIntEnable(YAW_PERIPH_BASE, YAW_GPIO_PINS);
    IntEnable(YAW_INT);
}

int32_t GetYaw() {
    return yaw;
}

int32_t GetYawDegrees() {
    int32_t tmp_yaw = GetYaw();
    int32_t degrees = tmp_yaw * 360 / NUMBER_SLOTS;
    return degrees;
}
