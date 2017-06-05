/**
 * @file switch.h
 *
 * @brief A module to operate the mode switch.
 */

/**
 * @defgroup switch_api SwitchApi
 * @{
 */

#ifndef SWITCH_H_
#define SWITCH_H_

/*
 * Change switch state only after NUM_POLLS consecutive readings have an
 * opposite value.
 */
#define NUM_POLLS 5

/**
 * An enumeration of the states of the slider switch.
 */
enum SwitchState {
    /**
     * The switch is in the downwards position.
     */
    SWITCH_DOWN,
    /**
     * The switch is in the upwards position.
     */
    SWITCH_UP
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

/** @} */
