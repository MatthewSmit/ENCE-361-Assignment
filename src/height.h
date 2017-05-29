/**
 * @file height.h
 */

#ifndef HEIGHT_H_
#define HEIGHT_H_

#define FULL_SCALE_RANGE 993 // 0.8 V sensor range
//#define FULL_SCALE_RANGE 1241 // 1.0 V sensor range

int32_t GetHeight(void);
int32_t GetHeightPercentage(void);
void UpdateHeight();
void HeightManagerInit(void);
void ZeroHeightTrigger(void);

#endif /* HEIGHT_H_ */
