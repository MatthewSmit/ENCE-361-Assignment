/**
 * @file oled_interface.c
 *
 * @brief A simple interface to the Orbit OLED library.
 */

#include "../lib/libOrbitOled/OrbitOled.h"
#include "../lib/libOrbitOled/delay.h"
#include "../lib/libOrbitOled/FillPat.h"
#include "../lib/libOrbitOled/LaunchPad.h"
#include "../lib/libOrbitOled/OrbitBoosterPackDefs.h"
#include "../lib/libOrbitOled/OrbitOled.h"
#include "../lib/libOrbitOled/OrbitOledChar.h"
#include "../lib/libOrbitOled/OrbitOledGrph.h"

#include "oled_interface.h"

void OledStringDraw(char *string_ptr, uint32_t x_char, uint32_t y_char) {
    OrbitOledSetCursor(x_char, y_char);
    OrbitOledPutString(string_ptr);
}

void OledInit(void) {
    /*
     * Initialize the OLED display
     */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); // Need signals on GPIOD
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // Need signals on GPIOE

    OrbitOledInit();
}

void OledClearBuffer(void) {
    OrbitOledClearBuffer();
}

