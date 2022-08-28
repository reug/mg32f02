#ifndef CORE_H
#define CORE_H

#include "MG32x02z__RegAddress.h"

/// Макрос 8-битного доступа к ячейке (регистру)
#define RB(addr)      (*(volatile uint8_t*)(addr))
/// Макрос 16-битного доступа к ячейке (регистру)
#define RH(addr)      (*(volatile uint16_t*)(addr))
/// Макрос 32-битного доступа к ячейке (регистру)
#define RW(addr)      (*(volatile uint32_t*)(addr))


#endif // CORE_H
