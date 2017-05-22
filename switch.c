/**
 * @file buttons.c
 *
 * @author Matthew Smit
 * @date 2017-05-20
 */

#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "switch.h"

/* UP Button */
#define SWITCH_PERIPH           SYSCTL_PERIPH_GPIOA
#define SWITCH_BASE             GPIO_PORTA_BASE
#define SWITCH_PIN              GPIO_PIN_7
#define SWITCH_DEFAULT          0

static bool current_state;
static bool event;
static uint16_t count;

void SwitchInit() {
    /* Switch is active high (default low) so are configured as pull down. */
    SysCtlPeripheralEnable(SWITCH_PERIPH);
    GPIOPinTypeGPIOInput(SWITCH_BASE, SWITCH_PIN);
    GPIOPadConfigSet(SWITCH_BASE, SWITCH_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

    current_state = GPIOPinRead(SWITCH_BASE, SWITCH_PIN);
    event = DOWN;
    count = 0;
}

void SwitchUpdate() {
    uint8_t current_value = GPIOPinRead(SWITCH_BASE, SWITCH_PIN);

    if (current_value != current_state) {
        count++;
        if  (count >= NUM_POLLS)
        {
            count = 0; // Reset the count
            current_state = current_value;
            event = current_value;
        }
    }
    else
        count = 0;
}

uint8_t GetSwitchEvent() {
    return event;
}
