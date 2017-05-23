/*
 * reset.c
 *
 *  Created on: 2017-05-23
 *      Author: Matthew Smit
 */

#include <stdbool.h>
#include <stdint.h>

#include "reset.h"

#include "serial_interface.h"

#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "utils/uartstdio.h"

#define RESET_PERIPH_GPIO       SYSCTL_PERIPH_GPIOA
#define RESET_PERIPH_BASE       GPIO_PORTA_BASE
#define RESET_PIN               GPIO_PIN_6
#define RESET_INT               INT_GPIOA

static void ResetHandler(void) {
    GPIOIntClear(RESET_PERIPH_BASE, RESET_PIN);
    UARTprintf("Resetting Device\n");
    SysCtlReset();
}

void InitReset(void) {
    SysCtlPeripheralEnable(RESET_PERIPH_GPIO);

    GPIOPinTypeGPIOInput(RESET_PERIPH_BASE, RESET_PIN);
    GPIOPadConfigSet(RESET_PERIPH_BASE, RESET_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIODirModeSet(RESET_PERIPH_BASE, RESET_PIN, GPIO_DIR_MODE_IN);
    GPIOIntTypeSet(RESET_PERIPH_BASE, RESET_PIN, GPIO_FALLING_EDGE);
    GPIOIntRegister(RESET_PERIPH_BASE, ResetHandler);
    GPIOIntEnable(RESET_PERIPH_BASE, RESET_PIN);
    IntEnable(RESET_INT);
}
