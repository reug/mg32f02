#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/// Идентификаторы таймеров
enum TimerID {
  TM00_id, TM01_id, TM10_id, TM16_id, TM20_id, TM26_id, TM36_id, TM_UNDEF
};

/// Base address
extern const uint32_t TM_BASE[8];

/// Bit mask for CSC_APB1_h0
extern const uint16_t TM_CSC[8];

/// IRQ
extern const uint8_t TM_IRQ[8];


/// Инициализация таймера (включение тактирования)
void tm_init(uint8_t tm_id);

/// Включение прерывания INT_TMx по флагам, указанным в flags согласно формату TMxx_INT
void tm_setup_int(uint8_t tm_id, uint32_t flags);

#endif // TIMER_H
