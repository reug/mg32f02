#ifndef RTC_H
#define RTC_H

#include <stdint.h>

/// Инициализация RTC
void rtc_init();

/// Включение прерывания INT_SYS по флагам, указанным в flags согласно формату RTC_INT
void rtc_set_int(uint8_t flags);

/// Разблокировка записи
void rtc_write_unlock();

/// Блокировка записи
void rtc_write_lock();

/// Подключение выхода. Задаются согласно формату RTC_CR0_b1:
/// RTC_OUT_LCK (7) | RTC_OUT_STA (6) | RTC_TS_TRGS (4-5) | RTC_OUT_SEL (0-1)
/// Разблокировка не требуется. При изменении RTC_OUT_STA также устанавливать RTC_OUT_LCK.
void rtc_set_out(uint8_t out_mode);

#endif // RTC_H
