/*
 * yaw_controller.h
 *
 *  Created on: 19/05/2017
 *      Author: daniel
 */

#ifndef YAW_CONTROLLER_H_
#define YAW_CONTROLLER_H_

#include "yaw_manager.h"
#include "pwm_output.h"
#include "pid.h"

/*
 * Public function prototypes
 */
void SetTargetYaw(int32_t yaw);
void YawControllerInit(void);
void UpdateYawController(uint32_t delta_t);

void TuneParamTailRotor(double my_k_p, double my_k_i, double my_k_d);

#endif /* YAW_CONTROLLER_H_ */
