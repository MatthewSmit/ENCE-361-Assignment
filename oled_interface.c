/**
 * @file orbitOledInterface.c
 */

#include "libOrbitOled/OrbitOled.h"
#include "libOrbitOled/delay.h"
#include "libOrbitOled/FillPat.h"
#include "libOrbitOled/LaunchPad.h"
#include "libOrbitOled/OrbitBoosterPackDefs.h"
#include "libOrbitOled/OrbitOled.h"
#include "libOrbitOled/OrbitOledChar.h"
#include "libOrbitOled/OrbitOledGrph.h"

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

