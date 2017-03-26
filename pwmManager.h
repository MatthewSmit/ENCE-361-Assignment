#pragma once
#include "config.h"

// Initialises PWM
void InitialisePWM();

// Sets the PWM Frequency and Duty Cycle
void SetPWM(int frequency, int dutyCycle);

// Enables the PWM output
void EnablePWM();
