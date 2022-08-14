#ifndef ADC_H
#define ADC_H

#include <stdint.h>


void adc_init();


void adc_start_one(uint8_t chn);

void adc_start_one_int(uint8_t chn);


int16_t adc_samp();


void adc_ivr24();


/// Включение ИОН на 1.4 В
void adc_vbuf();


///// Выбор внутреннего источника напряжения
//void adc_chsel();


#endif
