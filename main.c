#include "config.h"

#include <math.h>
#include <stdio.h>

#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

#include "buttons.h"
#include "OrbitOLEDInterface.h"
#include "pwmManager.h"
#include "yawManager.h"

#define PWM_START_RATE_HZ       150
#define PWM_START_DC            50

#define PIN_BUFFER_SIZE 10

// Range the SysTick timer counts in
static uint32_t sysTickRange = 0;

// Current MS count
static volatile uint32_t timeCount = 0;

// Last read MS time for pin changes
static volatile uint32_t pinChangeLastMS = 0;

// Current ticks in read representing pin changes
static volatile uint32_t pinTicks[PIN_BUFFER_SIZE] = {};
static volatile uint32_t pinTickPtr = 0;

void SysTickHandler();
void PinChangeHandler();

void InitialiseClock() {
	// Sets the clock to 80 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}

void InitialisePin() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOIntRegister(GPIO_PORTB_BASE, PinChangeHandler);

    GPIOPinTypeGPIOInput (GPIO_PORTB_BASE, GPIO_PIN_0);
    GPIOPadConfigSet (GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_RISING_EDGE);

    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0);
    IntEnable(INT_GPIOB);
}

void InitialiseSysTick() {
	// Sets the SysTick range to 1ms
	sysTickRange = SysCtlClockGet() / 1000;
    SysTickPeriodSet(sysTickRange);
    SysTickIntRegister(SysTickHandler);
    SysTickIntEnable();
    SysTickEnable();
}

void SysTickHandler() {
    timeCount++;
    updateButtons();
}

void PinChangeHandler() {
    static uint32_t lastSysTick = 0;

    //Clear Interrupt
    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0);

    // Calculate frequency using the MS counter plus the current SysTick value
    uint32_t sysTickValue = SysTickValueGet();
    uint32_t msDifference = timeCount - pinChangeLastMS;

    // SysTick counts down
    int32_t sysTickDifference = lastSysTick - sysTickValue;

    // Handle SysTick overlap
    if (sysTickDifference < 0)
    {
    	sysTickDifference += sysTickRange;
    	msDifference--;
    }

    pinChangeLastMS = timeCount;
    lastSysTick = sysTickValue;

    // Converts the ms + SysTick into SysTick values
    pinTicks[pinTickPtr] = sysTickDifference + msDifference * sysTickRange;
    pinTickPtr = (pinTickPtr + 1) % PIN_BUFFER_SIZE;
}

// Calculates frequency using pin change data
uint32_t CalculateFrequency() {

	// Sums and divides, uses 64 bit since it can get large
    uint64_t sum = 0;
    for (int i = 0; i < PIN_BUFFER_SIZE; i++)
        sum += pinTicks[i];
    sum /= PIN_BUFFER_SIZE;

    // Returns as a frequency
    return SysCtlClockGet() / sum;
}

// Returns true if no pin change has been read in the last 1s
bool PinChangeTimedOut() {
	uint32_t difference = timeCount - pinChangeLastMS;
	if (difference >= 1000)
	{
		pinChangeLastMS = timeCount - 1000;
		return true;
	}
	return false;
}

static bool lastButtonState[NUM_BUTTONS] = {};

bool IsButtonPressed(int button) {
	if (button < 0 || button >= NUM_BUTTONS)
		return false;

	return !lastButtonState[button] && checkButton(button);
}

void ButtonStateUpdate() {
	for (int i = 0; i < NUM_BUTTONS; i++)
		lastButtonState[i] = checkButton(i);
}

// Draws the frequency and duty cycle on the OLED
void Draw(uint32_t frequency, uint32_t dutyCycle) {
    OLEDStringDraw("Milestone 1", 0, 0);

    char stringBuffer[30];
    memset(stringBuffer, 0, sizeof(stringBuffer));

    float yaw = getYawDegrees();
    sprintf(stringBuffer, "Yaw: %d  ", (int)yaw);
    OLEDStringDraw(stringBuffer, 0, 1);
    memset(stringBuffer, 0, sizeof(stringBuffer));

    uint32_t height = 0;
    sprintf(stringBuffer, "Height: %d  ", height);
    OLEDStringDraw(stringBuffer, 0, 2);
    memset(stringBuffer, 0, sizeof(stringBuffer));

    //sprintf(stringBuffer, "Freq: %d Hz  ", frequency);
    //OLEDStringDraw(stringBuffer, 0, 2);
    //memset(stringBuffer, 0, sizeof(stringBuffer));

    sprintf(stringBuffer, "Duty Cycle: %d%% ", dutyCycle);
    OLEDStringDraw(stringBuffer, 0, 3);
}

int main(void) {
    InitialiseClock();
    InitialisePWM(PWM_START_RATE_HZ, PWM_START_DC);
    //InitialisePin();
    InitialiseSysTick();
    initButtons();
    initialiseYawManager();
    OLEDInitialise();

    EnablePWM();

    IntMasterEnable();

    uint32_t frequency = PWM_START_RATE_HZ;
    uint32_t dutyCycle = PWM_START_DC;
	
	while (true) {
		/*uint32_t newFrequency = CalculateFrequency();

		if (PinChangeTimedOut())
			newFrequency = PWM_START_RATE_HZ;

		if (newFrequency != frequency && newFrequency != 0)
		{
			// Clamp frequency to [100, 300] Hz
			if (newFrequency < 100)
				newFrequency = 100;

			if (newFrequency > 300)
				newFrequency = 300;

			SetPWM(newFrequency, dutyCycle);
			frequency = newFrequency;
		}*/

        if (IsButtonPressed(BUT_DOWN))
        {
            dutyCycle -= 5;
            if (dutyCycle < 5)
                dutyCycle = 5;

            SetPWM(frequency, dutyCycle);
        }

        if (IsButtonPressed(BUT_UP))
        {
            dutyCycle += 5;
            if (dutyCycle > 95)
                dutyCycle = 95;

            SetPWM(frequency, dutyCycle);
        }

		ButtonStateUpdate();

		Draw(frequency, dutyCycle);
	}
}
