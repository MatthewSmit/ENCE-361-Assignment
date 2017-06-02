/**
 * @file height.h
 *
 * @brief Module to acquire current height via the height sensor and trigger a
 * zero-height reading. Provides helper methods to get height.
 */
#ifndef HEIGHT_H_
#define HEIGHT_H_

/**
 * The range of the height sensor for a 100% height reading.
 */
#define FULL_SCALE_RANGE 993 // 0.8 V sensor range

/**
 * TODO
 *
 * @return The height.
 */
int32_t GetHeight(void);

/**
 * TODO
 *
 * @return The height as a percentage.
 */
int32_t GetHeightPercentage(void);

/**
 * TODO
 */
void UpdateHeight();

/**
 * TODO
 */
void HeightManagerInit(void);

/**
 * TODO
 */
void ZeroHeightTrigger(void);

#endif /* HEIGHT_H_ */
