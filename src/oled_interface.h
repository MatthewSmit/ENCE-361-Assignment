/**
 * @file oled_interface.h
 *
 * @brief A simple interface to the Orbit OLED library.
 */

#ifndef OLED_INTERFACE_H_
#define OLED_INTERFACE_H_

/**
 * Clear the OLED display buffer.
 */
void OledClearBuffer(void);

/**
 * Initialise the OLED display.
 */
void OledInit(void);

/**
 * Draw a string of character on the OLED display at the desired row and column.
 *
 * @param string_ptr The string to display.
 * @param x_char The display row.
 * @param y_char The display column.
 */
void OledStringDraw(char *string_ptr, uint32_t x_char, uint32_t y_char);

#endif /* OLED_INTERFACE_H_ */
