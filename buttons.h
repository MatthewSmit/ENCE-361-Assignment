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
 * Update all of the buttons and their state. This code is fast enough to be run in an ISR.
 */
void UpdateButtons(void);

/**
 * Get the button state.
 *
 * @param button_name BUT_UP, BUT_DOWN, BUT_LEFT or BUT_RIGHT.
 * @return state of the given button, RELEASED or PUSHED.
 */
uint8_t NumPushes(uint8_t button_name);

/**
 * Clears the buttons state.
 */
void ClearButtons(void);

#endif /* BUTTONS_H_ */
