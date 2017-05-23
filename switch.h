/*
 * @file buttons.c
 *
 * @author Matthew Smit
 * @date 2017-05-20
 */

#ifndef SWITCH_H_
#define SWITCH_H_

/* State changes only after so many consecutive readings have an opposite value */
#define NUM_POLLS 5

enum {SWITCH_DOWN, SWITCH_UP};

/**
 * Initialise the switch.
 */
void SwitchInit();

/**
 * Update the switch state. This code is fast enough to be run in an ISR.
 */
void UpdateSwitch();

/**
 * Get the switch event.
 *
 * @return DOWN or UP slide of the switch.
 */
uint8_t GetSwitchEvent();

#endif /* SWITCH_H_ */
