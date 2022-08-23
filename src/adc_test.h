#ifndef ADC_TEST_H
#define ADC_TEST_H

#include "adc.h"


/// Режим единичного измерения
void adc_test_one();

/// Режим сканирования нескольких каналов
void adc_test_scan();

/// Режим измерения с суммированием одного канала
void adc_test_sum();

/// Режим измерения с суммированием одного канала в фоне
void adc_test_sum_cont();

/// Тестирование ИОН на 2.40 В
void adc_test_ivr24();

/// Тест компаратора
void cmp_test();

/// Тест ИОН компаратора
void cmp_test_ivref();

/// Характеристика ИОН компаратора
void cmp_test_ivref_gen();

#endif // ADC_TEST_H
