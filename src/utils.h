#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include "api.h"
#include "uart.h"
#include "adc.h"
#include "cmp.h"
#include "core.h"
#include "hwcf.h"

/// Номер модуля URTx
#define PORT 0


/// Вывод 16-битного числа в URT<PORT> в hex и dec
void debug(char label, uint16_t d);

/// Вывод 32-битного числа в URT<PORT> в dec
void debug32(char label, uint32_t d);

/// Вывод 32-битного числа в URT<PORT> в hex
void debug32hex(char label, uint32_t d);

void debugbuf(uint8_t* buf, uint32_t len);

/// Включение светодиода D1 на 100 мс
void led1_flash();

/// Включение светодиода D2 на 100 мс
void led2_flash();

/// Переключение двух светодиодов по 250 мс
void led_blink();

inline
void led1_on() {RH(HW_LED1_SCH0) = HW_LED1_MASK;}

inline
void led1_off() {RH(HW_LED1_SCH1) = HW_LED1_MASK;}

inline
void led2_on() {RH(HW_LED2_SCH0) = HW_LED2_MASK;}

inline
void led2_off() {RH(HW_LED2_SCH1) = HW_LED2_MASK;}

#endif //  UTILS_H
