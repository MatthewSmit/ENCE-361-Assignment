/*
 * yaw_controller.h
 *
 *  Created on: 19/05/2017
 *      Author: daniel
 */

#ifndef YAW_CONTROLLER_H_
#define YAW_CONTROLLER_H_

#include "pwm_output.h"
#include "pid.h"
#include "yaw.h"

/*
 * Public function prototypes
 */
int32_t GetTargetYawDegrees(void);
void SetTargetYawDegrees(int32_t yaw);
int32_t GetTargetYaw(void);
void SetTargetYaw(int32_t yaw);
void YawControllerInit(void);
void UpdateYawController(uint32_t delta_t);

void TuneParamTailRotor(double my_k_p, double my_k_i, double my_k_d);

#endif /* YAW_CONTROLLER_H_ */
