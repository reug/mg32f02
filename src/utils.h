#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include "api.h"
#include "uart.h"
#include "adc.h"
#include "cmp.h"
#include "MG32x02z__RegAddress.h"

/// Номер модуля URTx
#define PORT 0


/// Вывод 16-битного числа в URT<PORT> в hex и dec
void debug(char label, uint16_t d);

/// Вывод 32-битного числа в URT<PORT> в dec
void debug32(char label, uint32_t d);

/// Вывод 32-битного числа в URT<PORT> в hex
void debug32hex(char label, uint32_t d);

/// Включение светодиода D1 на 100 мс
void led1_flash();

/// Включение светодиода D2 на 100 мс
void led2_flash();

/// Переключение двух светодиодов по 250 мс
void led_blink();


#endif //  UTILS_H
