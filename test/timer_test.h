#ifndef TIMER_TEST_H
#define TIMER_TEST_H

#include <stdint.h>


/// Режим Separate Mode.
/// khi - коэффициент счета для CT1, klo - коэффициент счета для CT2
void tm00_setup_separate(uint16_t khi, uint16_t klo);

/// Режим Cascade Mode (2x8 bit)
void tm00_setup_cascade(uint16_t khi, uint16_t klo);

/// Режим Full-Counter (16 bit)
void tm00_setup_fullcnt(uint32_t k);

/// Общая функция тестирования
void tm00_test();

/// Функция тестрования счетчиков на низкой частоте счета с выводом значений в терминал
void tm00_test_lowfreq_separate();

/// Функция тестрования счетчиков на низкой частоте счета с выводом значений в терминал
void tm00_test_lowfreq_fullcnt();


#endif //  TIMER_TEST_H
