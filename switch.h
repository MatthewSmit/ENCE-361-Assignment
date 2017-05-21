/*
 * @file buttons.c
 *
 * @author Matthew Smit
 * @date 2017-05-20
 */

#ifndef SWITCH_H_
#define SWITCH_H_

enum {DOWN, UP};

/**
 * Initialise the switch.
 */
void SwitchInit();

/**
 * Update the switch state. This code is fast enough to be run in an ISR.
 */
void SwitchUpdate();

/**
 * Get the switch state.
 *
 * @return state of the switch, DOWN or UP.
 */
uint8_t GetSwitchState();

#endif /* SWITCH_H_ */
