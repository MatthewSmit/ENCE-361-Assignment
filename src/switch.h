/**
 * @file switch.c
 *
 * @brief A module to operate the mode switch.
 */

#ifndef SWITCH_H_
#define SWITCH_H_

/*
 * State changes only after so many consecutive readings have an opposite value
 */
#define NUM_POLLS 5

enum {
    SWITCH_DOWN, SWITCH_UP
};

/**
 * Initialise the switch.
 */
void SwitchInit(void);

/**
 * Update the switch state.
 */
void UpdateSwitch();

/**
 * Get the switch event.
 *
 * @return DOWN or UP slide of the switch.
 */
uint8_t GetSwitchEvent(void);

#endif /* SWITCH_H_ */
