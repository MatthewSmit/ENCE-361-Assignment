/*
 * serial.c
 *
 *  Created on: 7/05/2017
 *      Author: daniel
 */

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#include "serialInterface.h"

#define UART_BASE               UART0_BASE
#define UART_GPIO_BASE          GPIO_PORTA_BASE
#define UART_GPIO_RX_CONFIG     GPIO_PA0_U0RX
#define UART_GPIO_TX_CONFIG     GPIO_PA1_U0TX
#define UART_GPIO_RX_PIN        GPIO_PIN_0
#define UART_GPIO_TX_PIN        GPIO_PIN_1
#define UART_PERIPH_UART        SYSCTL_PERIPH_UART0
#define UART_PIOSC_FREQUENCY    16000000

void initSerial(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(UART_GPIO_RX_CONFIG);
    GPIOPinConfigure(UART_GPIO_TX_CONFIG);

    /*
     * Enable UART0 so that we can configure the clock.
     */
    SysCtlPeripheralEnable(UART_PERIPH_UART);

    /*
     * Use the internal 16MHz oscillator as the UART clock source.
     */
    UARTClockSourceSet(UART_BASE, UART_CLOCK_PIOSC);

    GPIOPinTypeUART(UART_GPIO_BASE, UART_GPIO_RX_PIN | UART_GPIO_TX_PIN);

    uint32_t test = UARTClockSourceGet(UART_BASE);

    UARTStdioConfig(0, BAUD_RATE, UART_PIOSC_FREQUENCY);
}
