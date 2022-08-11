#include "adc.h"
#include "MG32x02z__RegAddress.h"


void adc_init() {
  *((volatile uint16_t*)CSC_KEY_h0) = 0xA217; // unlock access to CSC regs
  *(volatile uint8_t*)CSC_APB0_b0 |= 1; // CSC_ADC0_EN = 1
  *((volatile uint16_t*)CSC_KEY_h0) = 0x1111; // lock access to CSC regs
  *(volatile uint8_t*)ADC0_CR0_b0 = 1;// ADC0_RES_SEL = 0 (12bit), ADC0_EN = 1
  //*(volatile uint16_t*)PA_CR8_b0 = 0; // PA_IOM8 = 0 analog input (DEFAULT)

}


void adc_start_one(uint8_t chn) {
  *(volatile uint16_t*)ADC0_START_h0 = (chn << 8) | 1; // ADC0_CH_MUX = chn & ADC0_START = 1
}


int16_t adc_samp() {
  while ( ! (*(volatile uint8_t*)ADC0_STA_b0 & 0x08)); // waiting ADC0_E1CNVF==1
  return *(volatile uint16_t*)ADC0_DAT0_h0; // ADC0_DAT0
}
