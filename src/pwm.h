/**
 * @file pwm.h
 *
 * @author Daniel van Wichen
 * @date 25.03.2017
 */

#ifndef PWM_H_
#define PWM_H_

#define PWM_FREQUENCY 200

enum {MAIN_ROTOR, TAIL_ROTOR};

void PwmInit();
void SetPwmDutyCycle(uint8_t pwm_output, uint32_t duty_cycle);
uint32_t GetPwmDutyCycle(uint8_t pwm_output);
void PwmDisable(uint8_t pwm_output);
void PwmEnable(uint8_t pwm_output);

#endif /* PWM_H_ */
