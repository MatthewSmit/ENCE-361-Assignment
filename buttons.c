/**
 * @file buttons.c
 *
 * @author Daniel van Wichen
 * @date 25.03.2017
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "buttons.h"

/* UP Button */
#define BUT_UP_PERIPH           SYSCTL_PERIPH_GPIOE
#define BUT_UP_BASE             GPIO_PORTE_BASE
#define BUT_UP_PIN              GPIO_PIN_0
#define BUT_UP_DEFAULT          0

/* DOWN Button */
#define BUT_DOWN_PERIPH         SYSCTL_PERIPH_GPIOD
#define BUT_DOWN_BASE           GPIO_PORTD_BASE
#define BUT_DOWN_PIN            GPIO_PIN_2
#define BUT_DOWN_DEFAULT        0

/* LEFT Button */
#define BUT_LEFT_PERIPH         SYSCTL_PERIPH_GPIOF
#define BUT_LEFT_BASE           GPIO_PORTF_BASE
#define BUT_LEFT_PIN            GPIO_PIN_4
#define BUT_LEFT_DEFAULT        1

/* RIGHT Button */
#define BUT_RIGHT_PERIPH        SYSCTL_PERIPH_GPIOF
#define BUT_RIGHT_BASE          GPIO_PORTF_BASE
#define BUT_RIGHT_PIN           GPIO_PIN_0
#define BUT_RIGHT_DEFAULT       1

static bool default_state[NUM_BUTTONS];
static bool current_state[NUM_BUTTONS];
static uint16_t count[NUM_BUTTONS];
static uint8_t pushes[NUM_BUTTONS]

void initButtons (void)
{
    /* UP and DOWN buttons are active high (default low) so are configured as pull down. */
    SysCtlPeripheralEnable (BUT_UP_PERIPH);
    GPIOPinTypeGPIOInput (BUT_UP_BASE, BUT_UP_PIN);
    GPIOPadConfigSet (BUT_UP_BASE, BUT_UP_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

    SysCtlPeripheralEnable (BUT_DOWN_PERIPH);
    GPIOPinTypeGPIOInput (BUT_DOWN_BASE, BUT_DOWN_PIN);
    GPIOPadConfigSet (BUT_DOWN_BASE, BUT_DOWN_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

    /* LEFT and RIGHT buttons are active low (default high) so are configured as pull up. */
    SysCtlPeripheralEnable (BUT_LEFT_PERIPH);
    GPIOPinTypeGPIOInput (BUT_LEFT_BASE, BUT_LEFT_PIN);
    GPIOPadConfigSet (BUT_LEFT_BASE, BUT_LEFT_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    /* Unlock PF0 so we can change it to a GPIO input
    Once we have enabled (unlocked) the commit register then re-lock it
    to prevent further changes.  PF0 is muxed with NMI thus a special case. */
    HWREG(BUT_RIGHT_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(BUT_RIGHT_BASE + GPIO_O_CR) |= 0x01;
    HWREG(BUT_RIGHT_BASE + GPIO_O_LOCK) = 0;

    SysCtlPeripheralEnable (BUT_RIGHT_PERIPH);
    GPIOPinTypeGPIOInput (BUT_RIGHT_BASE, BUT_RIGHT_PIN);
    GPIOPadConfigSet (BUT_RIGHT_BASE, BUT_RIGHT_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    /* Setup the default states of the buttons */
    default_state[BUT_UP] = BUT_UP_DEFAULT;
    default_state[BUT_DOWN] = BUT_DOWN_DEFAULT;
    default_state[BUT_LEFT] = BUT_LEFT_DEFAULT;
    default_state[BUT_RIGHT] = BUT_RIGHT_DEFAULT;

    /* Reset other variables */
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        current_state[i] = default_state[i];
        count[i] = 0;
        pushes[i] = 0;
    }
}

void updateButtons (void)
{
    bool current_value[NUM_BUTTONS];
    current_value[BUT_UP] = GPIOPinRead (BUT_UP_BASE, BUT_UP_PIN);
    current_value[BUT_DOWN] = GPIOPinRead (BUT_DOWN_BASE, BUT_DOWN_PIN);
    current_value[BUT_LEFT] = GPIOPinRead (BUT_LEFT_BASE, BUT_LEFT_PIN);
    current_value[BUT_RIGHT] = GPIOPinRead (BUT_RIGHT_BASE, BUT_RIGHT_PIN);

    for (uint8_t i = 0; i < NUM_BUTTONS; i++)
    {
        if (current_value[i] != current_state[i]) {
            count[i]++;
            if  (count[i] >= NUM_POLLS)
            {
                count[i] = 0; // Reset the count
                current_state[i] = current_value[i];
                pushes[i]++;
            }
        }
        else
            count[i] = 0;
    }
}

//uint8_t checkButton (uint8_t button_name) {
//    if (current_state[button_name] == default_state[button_name])
//        return RELEASED;
//    else
//        return PUSHED;
//}

uint8_t numPushes(uint8_t button_name, bool reset) {
	uint8_t tmp_pushes = pushes[button_name];
	bool status = IntMasterDisable();
	if (reset)
		pushes[button_name] = 0;
	if (status)
		IntMasterEnable();
	return tmp_pushes;
}
