/**
 * @file pwm.h
 *
 * @brief PWM module to handle the power output for the Main and Tail rotors.
 */

#ifndef PWM_H_
#define PWM_H_

/**
 * The frequency of the PWM output signal (Hz).
 */
#define PWM_FREQUENCY 200

enum {MAIN_ROTOR, TAIL_ROTOR};

/**
 * TODO
 */
void PwmInit();

/**
 *
 * @param pwm_output
 * @param duty_cycle
 */
void SetPwmDutyCycle(uint8_t pwm_output, uint32_t duty_cycle);

/**
 * TODO
 *
 * @param pwm_output
 * @return
 */
uint32_t GetPwmDutyCycle(uint8_t pwm_output);

/**
 * TODO
 *
 * @param pwm_output
 */
void PwmDisable(uint8_t pwm_output);

/**
 * TODO
 *
 * @param pwm_output
 */
void PwmEnable(uint8_t pwm_output);

#endif /* PWM_H_ */
