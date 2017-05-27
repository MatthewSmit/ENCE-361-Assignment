/**
 * @file yaw_controller.h
 *
 */

#ifndef YAW_CONTROLLER_H_
#define YAW_CONTROLLER_H_

/*
 * Public function prototypes
 */
int32_t GetTargetYawDegrees(void);
void SetTargetYawDegrees(int32_t yaw);
int32_t GetTargetYaw(void);
void SetTargetYaw(int32_t yaw);
void YawControllerInit(void);
void UpdateYawController(uint32_t delta_t);

void TuneProportionalTailRotor(double gain);

#endif /* YAW_CONTROLLER_H_ */
