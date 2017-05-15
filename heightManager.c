/*
 * heightMonitor.c
 *
 *  Created on: 7/05/2017
 *      Author: daniel
 */

#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#include "circBuf.h"

#define FULL_SCALE_RANGE 993

static circBuf_t g_buffer;

static uint32_t zero_value = 0;

void ADCInteruptHandler(void) {
    uint32_t pui32ADC0Value[1];
    ADCSequenceDataGet(ADC0_BASE, 3, pui32ADC0Value);
    ADCIntClear(ADC0_BASE, 3);
    writeCircBuf(&g_buffer, pui32ADC0Value[0]);
}

void InitialiseHeightMonitor(void) {
    initCircBuf(&g_buffer, 1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_4);

    //
    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.  Each ADC module has 4 programmable sequences, sequence 0
    // to sequence 3.  This example is arbitrarily using sequence 3.
    //
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE | ADC_CTL_END);

    //
    // Since sample sequence 3 is now configured, it must be enabled.
    //
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCHardwareOversampleConfigure(ADC0_BASE, 64);

    //
    // Clear the interrupt status flag.  This is done to make sure the
    // interrupt flag is cleared before we sample.
    //
    ADCIntRegister(ADC0_BASE, 3, ADCInteruptHandler);
    ADCIntClear(ADC0_BASE, 3);

    ADCProcessorTrigger(ADC0_BASE, 3);

    while(!ADCIntStatus(ADC0_BASE, 3, false));

    ADCSequenceDataGet(ADC0_BASE, 3, &zero_value);
    ADCIntClear(ADC0_BASE, 3);

    for(int i = 0; i < g_buffer.size; i++) {
        writeCircBuf(&g_buffer, zero_value);
    }

    ADCIntEnable(ADC0_BASE, 3);
}

uint32_t getHeight(void) {
    // clamp between 0 and fullscale
//    return (uint32_t) readCircBuf(&g_buffer);
    uint32_t raw_height = readCircBuf(&g_buffer);
    int32_t adj_height = zero_value - raw_height;
    if (adj_height < 0) {
        return 0;
    } else if (adj_height > FULL_SCALE_RANGE) {
        return FULL_SCALE_RANGE;
    } else
        return adj_height;
}


uint32_t getHeightPercentage(void) {
//    return (uint32_t) readCircBuf(&g_buffer);
    uint32_t height = getHeight();
    return (height * 100 + FULL_SCALE_RANGE / 2) / (FULL_SCALE_RANGE);
}

void updateHeight(void) {
    ADCProcessorTrigger(ADC0_BASE, 3);
}
