#include "adc.h"
#include "MG32x02z__RegAddress.h"


void adc_init() {
  *(volatile uint16_t*)CSC_KEY_h0 = 0xA217; // unlock access to CSC regs
  *(volatile uint8_t*)CSC_APB0_b0 |= 1; // CSC_ADC0_EN = 1
  *(volatile uint16_t*)CSC_KEY_h0 = 0x1111; // lock access to CSC regs
  *(volatile uint8_t*)ADC0_CR0_b0 = 1;// ADC0_RES_SEL = 0 (12bit), ADC0_EN = 1
  //*(volatile uint8_t*)ADC0_CLK_b0 = (2 << 4); // ADC0_CK_DIV = 2 (DIV4);
  *(volatile uint8_t*)ADC0_CLK_b0 = (1 << 4); // ADC0_CK_DIV = 1 (DIV2);
}


void adc_start_one(uint8_t chn) {
  // ADC0_CH_MUX = chn & ADC0_START = 1
  *(volatile uint16_t*)ADC0_START_h0 = (chn << 8) | 1;
}


uint16_t adc_samp() {
  // waiting ADC0_E1CNVF==1
  while ( ! (*(volatile uint8_t*)ADC0_STA_b0 & 0x08));
  return *(volatile uint16_t*)ADC0_DAT0_h0; // ADC0_DAT0
}


void adc_ivr24() {
  // ADC0_IVREF_SEL = 1,  ADC0_IVR_EN = 1
  *(volatile uint8_t*)ADC0_ANA_b0 = (1 << 4) | 2;
}


void adc_vbuf() {
  *(volatile uint16_t*)PW_KEY_h0 = 0xA217; // unlock access to PW regs
  *(volatile uint8_t*)PW_CR0_b0 = 2; // PW_IVR_EN = 1
  *(volatile uint16_t*)PW_KEY_h0 = 0; // lock access to PW regs
}


void adc_tso(uint8_t ts_auto) {
  // ADC0_TS_AUTO = ts_auto, ADC0_TS_EN = 1
  *(volatile uint16_t*)ADC0_ANA_h0 |= (ts_auto << 15) | (1 << 3);
}


void adc_settime(uint8_t t) {
  if (t)
    *(volatile uint16_t*)ADC0_ANA_h0 |= (1 << 14); // ADC0_CONV_TIME = 1 (30)
  else
    *(volatile uint16_t*)ADC0_ANA_h0 &= ~(1 << 14); // ADC0_CONV_TIME = 0 (24)
}


uint16_t adc_measure_sum(uint8_t chn) {
  // Настройка режима суммирования в аккумуляторе SUM0:
  *(volatile uint8_t*)ADC0_CR1_b2 = 16; // sum of 8 (max 64) , ADC0_SUM_MDS =0 (single) - DEFAULT
  *(volatile uint8_t*)ADC0_MSK_b2 = chn & 0x0F; // ADC0_SUM0_MUX = chn
  *(volatile uint16_t*)ADC0_SUM0_h0 = 0;  // clear sum

  // Полусофтовый режим:
  do {
    // Запускаем одиночное измерение и ожидаем его завершение:
    *(volatile uint16_t*)ADC0_START_h0 = (chn << 8) | 1; // ADC0_CH_MUX = chn, ADC0_START = 1
    while ( ! (*(volatile uint8_t*)ADC0_STA_b0 & 0x08)); // waiting ADC0_E1CNVF==1
    *(volatile uint8_t*)ADC0_STA_b0 |= 0x08;  // clear ADC0_E1CNVF flag
  }
  // Ожидаем заданное число измерений по активации флага ADC0_SUMCF
  while ( ! ( *(volatile uint16_t*)ADC0_STA_h0 & (1 << 14) ) ); // ADC0_SUMCF == 1 ?

  //*(volatile uint16_t*)ADC0_STA_h0 = 0xFFFF;  // clear all flags ЛИБО ВСЕ
  *(volatile uint8_t*)ADC0_STA_b1 = 0xE0;  // clear all flags

  // Обработка результата: деление на 16
  return *(volatile uint16_t*)ADC0_SUM0_h0 >> 4; // ADC0_DAT0 div 16
}


uint16_t adc_measure_sum_cont(uint8_t chn) {
  // Настройка режима суммирования в аккумуляторе SUM0:
  // ВНИМАНИЕ: Обязательно устанавливать ADC0_SOVR_MDS, иначе результат суммирования будет непредсказуем!
  // Sum of 16 (max 64), ADC0_SUM_MDS =0 (single) - DEFAULT, ADC0_SOVR_MDS=1
  *(volatile uint32_t*)ADC0_CR1_w = (16 << 16) | (1 << 9);
  *(volatile uint8_t*)ADC0_MSK_b2 = chn & 0x0F; // ADC0_SUM0_MUX = chn
  *(volatile uint16_t*)ADC0_SUM0_h0 = 0;  // clear sum

  // Запускаем серию одиночных измерений
  //ADC0_TRG_CONT=1, ADC0_CH_MUX = chn, ADC0_START = 1
  *(volatile uint32_t*)ADC0_START_w = (1 << 19) | (chn << 8) | 1;

  // Ожидаем заданное число измерений по активации флага ADC0_SUMCF
  while (! ( *(volatile uint16_t*)ADC0_STA_h0 & (1 << 14) ) ); // ADC0_SUMCF == 1 ?

  // Эти действия обязательны! Их последовательность не столь критична.
  *(volatile uint8_t*)ADC0_CR1_b2 = 0; // ADC0_SUM_NUM=0
  *(volatile uint32_t*)ADC0_START_w = 0; // ADC0_TRG_CONT=0, ADC0_START=0
  *(volatile uint16_t*)ADC0_STA_h0 = 0xFFFF;  // clear all flags

  // Обработка результата: деление на 16
  //return *(volatile uint16_t*)ADC0_SUM0_h0 >> 4; // ADC0_DAT0 div 16
  return *(volatile uint16_t*)ADC0_SUM0_h0; // ADC0_DAT0  БЕЗ ДЕЛЕНИЯ ТОЛЬКО ДЛЯ ТЕСТА
}


void adc_start_mask(uint16_t mask) {
  *(volatile uint16_t*)ADC0_MSK_h0 = mask;
  // ADC0_CONV_MDS = 1 (Scan), ADC0_TRG_CONT = 1, ADC0_START = 1
  *(volatile uint32_t*)ADC0_START_w = (1 << 24) | (1 << 19) | 1;
}


// TODO
uint16_t adc_temperature_cal(uint16_t d) {
  uint32_t tc = *(volatile uint32_t*)ADC0_TCAL_w;
  uint32_t t0 = tc & 0x0FFF;  // ADC0_TCAL0
  uint32_t t1 = (tc >> 16) & 0x0FFF; // ADC0_TCAL1

}
