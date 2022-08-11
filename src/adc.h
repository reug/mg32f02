#ifndef ADC_H
#define ADC_H

#include <stdint.h>


void adc_init();


void adc_start_one(uint8_t chn);


int16_t adc_samp();


#endif
