/**
 * @file flight_controller.h
 *
 * @brief Handles moving between flight modes.
 *
 * For example, the heli must initiate a landing sequence if it was previously
 * flying if the mode switch was moved to the UP position. Also, this module
 * a convenient method to get the current flight mode.
 */

/**
 * @defgroup flight_api FlightController
 * @{
 */

#ifndef FLIGHT_CONTROLLER_H_
#define FLIGHT_CONTROLLER_H_

enum {
    LANDED, INIT, FLYING, LANDING
};

/**
 * Get this.
 */
void FlightControllerInit(void);

/**
 * Get this.
 */
void UpdateFlightMode();

/**
 * Get a string representation of the current flight mode.
 *
 * @return a string representing the flight mode
 */
const char* GetFlightMode(void);

/**
 * Get this
 */
void PriorityTaskInit(void);

/**
 * Get this
 */
void PriorityTaskEnable(void);

#endif /* FLIGHT_CONTROLLER_H_ */

/** @{ */
