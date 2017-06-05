/**
 * @file pwm.h
 *
 * @brief PWM module to handle the power output for the main and tail rotors.
 */

/**
 * @defgroup pwm_api PwmOutput
 *
 * PWM module to handle the power output for the main and tail rotors.
 * @{
 */

#ifndef PWM_H_
#define PWM_H_

/*
 * The frequency of the PWM output signal (Hz).
 */
#define PWM_FREQUENCY 200

/**
 * An enumeration for determining which PWM output to configure.
 */
enum PwmOutput {
    /**
     * The main rotor PWM output.
     */
    MAIN_ROTOR,
    /**
     * The tail rotor PWM output.
     */
    TAIL_ROTOR
};

/**
 * Initialise both of the pwm outputs.
 */
void PwmInit();

/**
 * Set the duty cycle of the PWM output in the range 2-98%.
 *
 * @param pwm_output The PWM output to configure.
 * @param duty_cycle The desired duty cycle, in the range 2-98%.
 */
void SetPwmDutyCycle(uint8_t pwm_output, uint32_t duty_cycle);

/**
 * Get the current PWM duty cycle for the given PWM output.
 *
 * @param pwm_output The PWM output.
 * @return The current PWM duty cycle as percentage.
 */
uint32_t GetPwmDutyCycle(uint8_t pwm_output);

/**
 * Disable the given PWM output.
 *
 * @param pwm_output The PWM output to configure.
 */
void PwmDisable(uint8_t pwm_output);

/**
 * Enable the given PWM output.
 *
 * @param pwm_output The PWM output to configure.
 */
void PwmEnable(uint8_t pwm_output);

#endif /* PWM_H_ */

/** @} */
