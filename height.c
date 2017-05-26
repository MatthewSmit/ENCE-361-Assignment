/**
 * @file heightManager.c
 *
 */

#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#define FULL_SCALE_RANGE 993 // 0.8 V sensor range
//#define FULL_SCALE_RANGE 1241 // 1.0 V sensor range

#define ADC_GPIO_BASE       GPIO_PORTE_BASE
#define ADC_GPIO_PIN        GPIO_PIN_4
#define ADC_BASE            ADC0_BASE
#define ADC_SEQUENCE        3
#define ADC_CHANNEL         ADC_CTL_CH9
#define ADC_PERIPH_ADC      SYSCTL_PERIPH_ADC0
#define ADC_PERIPH_GPIO     SYSCTL_PERIPH_GPIOE

static uint32_t zero_reading;
static bool ref_found = false;
static volatile uint32_t adc_val;

void AdcHandler() {
    uint32_t adc_buf[1];
    ADCSequenceDataGet(ADC_BASE, ADC_SEQUENCE, adc_buf);
    ADCIntClear(ADC_BASE, ADC_SEQUENCE);
    adc_val = adc_buf[0];
}

void HeightManagerInit() {
    SysCtlPeripheralEnable(ADC_PERIPH_ADC);
    SysCtlPeripheralEnable(ADC_PERIPH_GPIO);

    GPIOPinTypeADC(ADC_GPIO_BASE, ADC_GPIO_PIN);

    ADCIntRegister(ADC_BASE, ADC_SEQUENCE, AdcHandler);

    ADCIntClear(ADC_BASE, ADC_SEQUENCE);
    ADCIntEnable(ADC_BASE, ADC_SEQUENCE);

    /* Set to timer trigger for periodic height reading. */
    ADCSequenceDisable(ADC_BASE, ADC_SEQUENCE);
    ADCSequenceConfigure(ADC_BASE, ADC_SEQUENCE, ADC_TRIGGER_TIMER, 0);
    ADCSequenceStepConfigure(ADC_BASE, ADC_SEQUENCE, 0, ADC_CHANNEL | ADC_CTL_IE | ADC_CTL_END);
    ADCHardwareOversampleConfigure(ADC_BASE, 64);
    ADCSequenceEnable(ADC_BASE, ADC_SEQUENCE);
}

void ZeroHeightTrigger(void) {
    /* Set to manual trigger so we can get a zero height reading. */
    ADCSequenceDisable(ADC_BASE, ADC_SEQUENCE);
    ADCSequenceConfigure(ADC_BASE, ADC_SEQUENCE, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceEnable(ADC_BASE, ADC_SEQUENCE);

    ADCIntDisable(ADC_BASE, ADC_SEQUENCE);
    ADCIntClear(ADC_BASE, ADC_SEQUENCE);

    ADCProcessorTrigger(ADC_BASE, ADC_SEQUENCE);
    while (!ADCIntStatus(ADC_BASE, ADC_SEQUENCE, false)) {
    }
    ADCSequenceDataGet(ADC_BASE, ADC_SEQUENCE, &zero_reading);
    adc_val = zero_reading;
    ref_found = true;

    /* Set back to timer trigger for periodic height reading. */
    ADCSequenceDisable(ADC_BASE, ADC_SEQUENCE);
    ADCSequenceConfigure(ADC_BASE, ADC_SEQUENCE, ADC_TRIGGER_TIMER, 0);
    ADCSequenceEnable(ADC_BASE, ADC_SEQUENCE);

    ADCIntEnable(ADC_BASE, ADC_SEQUENCE);
}

int32_t GetHeight() {
    if (ref_found) {
        return zero_reading - adc_val;
    } else {
        return 0;
    }
}

int32_t GetHeightPercentage() {
    return (GetHeight() * 100 + FULL_SCALE_RANGE / 2) / (FULL_SCALE_RANGE);
}

void UpdateHeight(void) {
    ADCProcessorTrigger(ADC_BASE, ADC_SEQUENCE);
}
