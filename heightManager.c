/**
 * @file heightManager.c
 *
 */

//TODO GENERALISED adc base and gpio defns

#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#define FULL_SCALE_RANGE 993

//static circBuf_t g_buffer;

static uint32_t zero_reading;
static uint32_t adc_val;

void AdcHandler() {
    uint32_t pui32Buffer[1];
    ADCSequenceDataGet(ADC0_BASE, 3, pui32Buffer);
    ADCIntClear(ADC0_BASE, 3);
    adc_val = pui32Buffer[0];
}

void InitialiseHeightMonitor() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_4);

    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);

    ADCHardwareOversampleConfigure(ADC0_BASE, 64);

    ADCIntRegister(ADC0_BASE, 3, AdcHandler);
    ADCIntClear(ADC0_BASE, 3);

    ADCProcessorTrigger(ADC0_BASE, 3);

    while (!ADCIntStatus(ADC0_BASE, 3, false));

    ADCSequenceDataGet(ADC0_BASE, 3, &zero_reading);
    ADCIntClear(ADC0_BASE, 3);

    ADCIntEnable(ADC0_BASE, 3);
}

int32_t GetHeight() {
    int32_t offset_height = zero_reading - adc_val;
    return offset_height;
}

int32_t GetHeightPercentage() {
    int32_t height = GetHeight();
    return (height * 100 + FULL_SCALE_RANGE / 2) / (FULL_SCALE_RANGE);
}

void UpdateHeight(void) {
    ADCProcessorTrigger(ADC0_BASE, 3);
}
