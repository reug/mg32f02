#ifndef ADC_H
#define ADC_H

#include <stdint.h>


/// Инициализация модуля АЦП
void adc_init();

/// Запуск одиночного измерения внешнего канала
void adc_start_one(uint8_t chn);

/// Запуск одиночного измерения внутреннего канала
void adc_start_one_int(uint8_t chn);

/// Считывание результата измерения
uint16_t adc_samp();

/// Включение встроенного ИОН на 2.4 В
void adc_ivr24();

/// Включение ИОН на 1.4 В
void adc_vbuf();

/// Установка времени измерения 24 (0) или 30 (1)
void adc_settime(uint8_t t);

/// Запуск и получение результата с суммированием 16 измерений.
/// chn - номер канала (внутренний или внешний).
uint16_t adc_measure_sum(uint8_t chn);

uint16_t adc_measure_sum_cont(uint8_t chn);

#endif
