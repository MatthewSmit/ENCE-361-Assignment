/**
 * @file buttons.c
 *
 * @brief A module to operate the buttons.
 */

#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"

#include "buttons.h"

/*
 * Up button definitions.
 */
#define BTN_UP_PERIPH           SYSCTL_PERIPH_GPIOE
#define BTN_UP_BASE             GPIO_PORTE_BASE
#define BTN_UP_PIN              GPIO_PIN_0
#define BTN_UP_DEFAULT          0

/*
 * Down button definitions.
 */
#define BTN_DOWN_PERIPH         SYSCTL_PERIPH_GPIOD
#define BTN_DOWN_BASE           GPIO_PORTD_BASE
#define BTN_DOWN_PIN            GPIO_PIN_2
#define BTN_DOWN_DEFAULT        0

/*
 * Left button definitions.
 */
#define BTN_LEFT_PERIPH         SYSCTL_PERIPH_GPIOF
#define BTN_LEFT_BASE           GPIO_PORTF_BASE
#define BTN_LEFT_PIN            GPIO_PIN_4
#define BTN_LEFT_DEFAULT        1

/*
 * Right button definitions.
 */
#define BTN_RIGHT_PERIPH        SYSCTL_PERIPH_GPIOF
#define BTN_RIGHT_BASE          GPIO_PORTF_BASE
#define BTN_RIGHT_PIN           GPIO_PIN_0
#define BTN_RIGHT_DEFAULT       1

static bool default_state[NUM_BUTTONS];
static bool current_state[NUM_BUTTONS];
static uint16_t count[NUM_BUTTONS];
static uint8_t pushes[NUM_BUTTONS];

void ButtonsInit(void) {
    /* UP and DOWN buttons are active high (default low) so are configured as pull down. */
    SysCtlPeripheralEnable(BTN_UP_PERIPH);
    GPIOPinTypeGPIOInput(BTN_UP_BASE, BTN_UP_PIN);
    GPIOPadConfigSet(BTN_UP_BASE, BTN_UP_PIN, GPIO_STRENGTH_2MA,
            GPIO_PIN_TYPE_STD_WPD);

    SysCtlPeripheralEnable(BTN_DOWN_PERIPH);
    GPIOPinTypeGPIOInput(BTN_DOWN_BASE, BTN_DOWN_PIN);
    GPIOPadConfigSet(BTN_DOWN_BASE, BTN_DOWN_PIN, GPIO_STRENGTH_2MA,
            GPIO_PIN_TYPE_STD_WPD);

    /* LEFT and RIGHT buttons are active low (default high) so are configured as pull up. */
    SysCtlPeripheralEnable(BTN_LEFT_PERIPH);
    GPIOPinTypeGPIOInput(BTN_LEFT_BASE, BTN_LEFT_PIN);
    GPIOPadConfigSet(BTN_LEFT_BASE, BTN_LEFT_PIN, GPIO_STRENGTH_2MA,
            GPIO_PIN_TYPE_STD_WPU);

    /*
     * Unlock PF0 so we can change it to a GPIO input.
     * Once we have enabled (unlocked) the commit register then re-lock it
     * to prevent further changes.  PF0 is muxed with NMI thus a special case.
     */
    HWREG(BTN_RIGHT_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(BTN_RIGHT_BASE + GPIO_O_CR) |= 0x01;
    HWREG(BTN_RIGHT_BASE + GPIO_O_LOCK) = 0;

    SysCtlPeripheralEnable(BTN_RIGHT_PERIPH);
    GPIOPinTypeGPIOInput(BTN_RIGHT_BASE, BTN_RIGHT_PIN);
    GPIOPadConfigSet(BTN_RIGHT_BASE, BTN_RIGHT_PIN, GPIO_STRENGTH_2MA,
            GPIO_PIN_TYPE_STD_WPU);

    /* Setup the default states of the buttons */
    default_state[BTN_UP] = BTN_UP_DEFAULT;
    default_state[BTN_DOWN] = BTN_DOWN_DEFAULT;
    default_state[BTN_LEFT] = BTN_LEFT_DEFAULT;
    default_state[BTN_RIGHT] = BTN_RIGHT_DEFAULT;

    /* Reset other variables */
    for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
        current_state[i] = default_state[i];
        count[i] = 0;
        pushes[i] = 0;
    }
}

void UpdateButtons() {
    bool current_value[NUM_BUTTONS];

    current_value[BTN_UP] = GPIOPinRead(BTN_UP_BASE, BTN_UP_PIN);
    current_value[BTN_DOWN] = GPIOPinRead(BTN_DOWN_BASE, BTN_DOWN_PIN);
    current_value[BTN_LEFT] = GPIOPinRead(BTN_LEFT_BASE, BTN_LEFT_PIN);
    current_value[BTN_RIGHT] = GPIOPinRead(BTN_RIGHT_BASE, BTN_RIGHT_PIN);

    for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
        if (current_value[i] != current_state[i]) {
            count[i]++;
            if (count[i] >= NUM_POLLS) {
                count[i] = 0; // Reset the count
                if (current_state[i] == default_state[i])
                    pushes[i]++;
                current_state[i] = current_value[i];
            }
        } else {
            count[i] = 0;
        }
    }
}

uint8_t NumPushes(uint8_t button_name) {
    bool was_disabled = IntMasterDisable();

    uint8_t tmp_pushes = pushes[button_name];
    pushes[button_name] = 0;

    if (!was_disabled)
        IntMasterEnable();
    return tmp_pushes;
}

void ResetPushes(void) {
    bool was_disabled = IntMasterDisable();

    for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
        current_state[i] = default_state[i];
        count[i] = 0;
        pushes[i] = 0;
    }

    if (!was_disabled)
        IntMasterEnable();
}
