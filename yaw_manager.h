#ifndef YAW_MANAGER_H_
#define YAW_MANAGER_H_

void YawManagerInit(void);
int32_t GetYaw(void);
int32_t GetYawDegrees(void);
int32_t GetClosestYawRef(int32_t yaw);
void YawRefTrigger(void);
bool YawRefFound(void);

#endif
