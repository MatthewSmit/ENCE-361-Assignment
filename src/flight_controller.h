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

/**
 * Initialise the flight controller module.
 */
void FlightControllerInit(void);

/**
 * U
 */
void UpdateFlightMode();

/**
 * Get a string representation of the current flight mode.
 *
 * @return a string representing the flight mode
 */
const char* GetFlightMode(void);

/**
 * Initialise the priority task sequencer. This timer handles updating of the
 * pid controllers and height.
 */
void PriorityTaskInit(void);

/**
 * Enable the priority task sequencer.
 */
void PriorityTaskEnable(void);

/**
 * Disable the priority task sequencer.
 */
void PriorityTaskDisable(void);

#endif /* FLIGHT_CONTROLLER_H_ */

/** @} */
