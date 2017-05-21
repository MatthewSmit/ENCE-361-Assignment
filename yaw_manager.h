#ifndef YAW_MANAGER_H_
#define YAW_MANAGER_H_

void YawManagerInit(void);
int32_t GetYaw(void);
int32_t GetYawDegrees(void);
void YawRefTrigger(void);
bool YawRefFound(void);

#endif
