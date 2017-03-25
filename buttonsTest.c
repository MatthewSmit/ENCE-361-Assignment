/*
 * buttonsTest.c
 *
 * Test that my button debouncing works.
 */



#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

#include "OrbitOLEDInterface.h"

#include "buttons.h"

#define SYSTICK_RATE_HZ 1000

void SysTickHandler (void);

/*
 * Initialize the clock and peripherals.
 */
void initialize (void) {
    /* Clock rate of 20 MHz */
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysTickPeriodSet (SysCtlClockGet() / SYSTICK_RATE_HZ);
    SysTickIntRegister (SysTickHandler);
    SysTickIntEnable ();
    SysTickEnable ();

    initButtons ();
    OLEDInitialise();

    IntMasterEnable();
}

void SysTickHandler (void) {
    updateButtons ();
}

int main(void) {
    initialize();

    uint16_t button = NUM_BUTTONS;

    while (1)
    {
//        updateButtons ();
        button = checkButton (BUT_RIGHT);

        char string[5];
        sprintf (string, "%s   ", button == RELEASED ? "RELEASED" : "PUSHED");
        OLEDStringDraw (string, 0, 0);
    }
}
