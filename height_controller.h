/**
 * @file HeightController.h
 *
 */

#ifndef HEIGHT_CONTROLLER_H_
#define HEIGHT_CONTROLLER_H_

/*
 * Public function prototypes
 */
void SetTargetHeight(uint32_t height);
uint32_t GetTargetHeight(void);
void HeightControllerInit(void);
void UpdateHeightController(uint32_t delta_t);

void TuneProportionalMainRotor(double gain);

#endif
