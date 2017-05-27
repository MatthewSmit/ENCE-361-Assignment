/**
 * @file orbitOledInterface.h
 */

#ifndef ORBIT_OLED_INTERFACE_H_
#define ORBIT_OLED_INTERFACE_H_

void OledClearBuffer(void);
void OledInit(void);
void OledStringDraw(char *string_ptr, uint32_t x_char, uint32_t y_char);

#endif /* ORBIT_OLED_INTERFACE_H_ */
