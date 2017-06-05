/**
 * @file buttons.h
 *
 * @brief A module to operate the buttons.
 */

/**
 * @defgroup buttons_api ButtonsApi
 * @{
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

/*
 * Change button state only after NUM_POLLS consecutive readings have an
 * opposite value.
 */
#define NUM_POLLS 5

/**
 * The buttons.
 */
enum Button {
    /**
     * The UP button.
     */
    BTN_UP,
    /**
     * The DOWN button.
     */
    BTN_DOWN,
    /**
     * The LEFT button.
     */
    BTN_LEFT,
    /**
     * The RIGHT button.
     */
    BTN_RIGHT,
    /**
     * The total number of buttons.
     */
    NUM_BUTTONS
};

/**
 * Initialise the buttons.
 */
void ButtonsInit(void);

/**
 * Update all of the buttons and their state.
 */
void UpdateButtons();

/**
 * Gets the number of pushes for a given button and resets the push count.
 *
 * @param button_name One of #BTN_UP, #BTN_DOWN, #BTN_LEFT, or #BTN_RIGHT.
 * @return The number of pushes for the given button since this function was last called.
 */
uint8_t NumPushes(uint8_t button_name);

/**
 * Reset the push count for all buttons.
 */
void ResetPushes(void);

#endif /* BUTTONS_H_ */

/** @} */
