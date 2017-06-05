/**
 * @file serial_interface.h
 *
 * @brief Serial UART interface.
 */
#ifndef SERIAL_INTERFACE_H_
#define SERIAL_INTERFACE_H_

/*
 * UART baud rate (Hz).
 */
#define BAUD_RATE 9600

/**
 * Initialise the UART serial interface. To print a string the UART, you can use
 * the UARTprintf function.
 */
void SerialInit();

#endif /* SERIAL_INTERFACE_H_ */
