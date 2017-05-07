/*
 * heightMonitor.h
 *
 *  Created on: 7/05/2017
 *      Author: daniel
 */

#include <stdint.h>

#ifndef HEIGHTMONITOR_H_
#define HEIGHTMONITOR_H_

uint32_t getHeight(void);
uint32_t getHeightPercentage(void);
void updateHeight(void);
void initHeightMonitor(void);

#endif /* HEIGHTMONITOR_H_ */
