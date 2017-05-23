/*
 * flight_controller.h
 *
 *  Created on: May 22, 2017
 *      Author: dpv11
 */

#ifndef FLIGHT_CONTROLLER_H_
#define FLIGHT_CONTROLLER_H_

enum {
    LANDED, INIT, FLYING, LANDING
};

//void PriorityTaskInit(void);
void FlightControllerInit(void);
void UpdateFlightMode();
uint8_t GetFlightMode(void);

#endif /* FLIGHT_CONTROLLER_H_ */
