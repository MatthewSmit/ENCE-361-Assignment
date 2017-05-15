/**
 * @file pwm_manager.h
 *
 * @author Daniel van Wichen
 * @date 25.03.2017
 */

#ifndef PWM_OUTPUT_H_
#define PWM_OUTPUT_H_

#define PWM_FREQUENCY 200

enum {MAIN_ROTOR, TAIL_ROTOR};

void InitialisePwm();
void SetPwmDutyCycle(uint8_t pwm_output, uint32_t duty_cycle);
void PwmDisable(uint8_t pwm_output);
void PwmEnable(uint8_t pwm_output);

#endif
