/**
 * @file pwm_manager.h
 *
 * @author Daniel van Wichen
 * @date 25.03.2017
 */

#ifndef PWM_OUTPUT_H_
#define PWM_OUTPUT_H_

#define PWM_FREQUENCY 200

enum pwm_outputs {MAIN_ROTOR, TAIL_ROTOR};

void init_pwm(void);
void pwm_duty_cycle_set(uint8_t pwm_output, uint32_t duty_cycle);
void pwm_enable(uint8_t pwm_output);
void pwm_disable(uint8_t pwm_output);

#endif
