#ifndef NCO_HPP
#define NCO_HPP

#include <stdint.h>

/// Режим работы модуля NCO
enum NCOMode {
  NCO_MODE_FDC = 0x00,  ///< Fixed Duty Cycle Mode
  NCO_MODE_PFM = 0x04   ///< Pulse Frequency Mode
};


/// Инициализация NCO
void nco_init(uint8_t mode);

/// Установка коэффициента умножения частоты (3..(2^19-1))
void nco_set(uint32_t k);

#endif // NCO_HPP
