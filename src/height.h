/**
 * @file height.h
 *
 * @brief Module to acquire current height via the height sensor and trigger a
 * zero-height reading. Provides helper methods to get the current height.
 */

/**
 * @defgroup height_api HeightApi
 *
 * Module to acquire current height via the height sensor and trigger a
 * zero-height reading. Provides helper methods to get the current height.
 * @{
 */

#ifndef HEIGHT_H_
#define HEIGHT_H_

/*
 * The range of the height sensor for a 100% height reading.
 *
 * This translates to a 0.8 V sensor range using the 12-bit ADC peripheral.
 */
#define FULL_SCALE_RANGE 993

/**
 * Get the current height. Retrieve the sensor reading after it has been offset
 * the zeroed sensor reading.
 *
 * @return The height.
 */
int32_t GetHeight(void);

/**
 * Get the current height as a percentage.
 *
 * @return The height as a percentage.
 */
int32_t GetHeightPercentage(void);

/**
 * Trigger the current height reading to be updated. No longer used in favour of
 * direct triggering via timer timeout flag.
 */
void UpdateHeight();

/**
 * Initialise the height sensor peripherals and ports.
 */
void HeightManagerInit(void);

/**
 * Trigger a zero height reading to be used as a reference for subsequent height
 * readings.
 */
void ZeroHeightTrigger(void);

#endif /* HEIGHT_H_ */

/** @} */
