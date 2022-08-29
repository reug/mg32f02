#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/// Инициализация TM00
void tm00_init();

/// Включение прерывания INT_TM0x по флагам, указанным в flags согласно формату TM00_INT
void tm00_set_int(uint8_t flags);

#endif // TIMER_H
