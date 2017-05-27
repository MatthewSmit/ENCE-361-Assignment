/**
 * @file buttons.h
 *
 * @author Daniel van Wichen
 * @date 25.03.2017
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

/* State changes only after so many consecutive readings have an opposite value */
#define NUM_POLLS 5

enum {BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT, NUM_BUTTONS};
enum {RELEASED, PUSHED};

/**
 * Initialise the buttons.
 */
void ButtonsInit(void);

/**
 * Update all of the buttons and their state.
 */
void UpdateButtons();

/**
 * Gets the number of pushes for a given button and resets the counter.
 *
 * @param button_name BUT_UP, BUT_DOWN, BUT_LEFT or BUT_RIGHT.
 * @return The number of pushes for the given button.
 */
uint8_t NumPushes(uint8_t button_name);

/**
 * Reset the buttons.
 */
void ResetPushes(void);

#endif /* BUTTONS_H_ */
