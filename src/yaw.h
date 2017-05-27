/**
 * @file yaw.h
 */

#ifndef YAW_H_
#define YAW_H_

#define NUMBER_SLOTS            112
#define YAW_FULL_ROTATION       (NUMBER_SLOTS * 4)

void YawManagerInit(void);
int32_t GetYaw(void);
int32_t GetYawDegrees(void);
int32_t GetClosestYawRef(int32_t yaw);
void YawRefTrigger(void);
bool YawRefFound(void);

#endif /* YAW_H_ */
