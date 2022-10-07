// DS3231 library for MG32F02 MCUs (ARM Cortex-M0)
// by reug@mail.ru, 2022

#ifndef DS3231_H
#define DS3231_H

#include <stdint.h>
#include "i2c.h"

// Custom configuration:
#define DS3231_PORT   I2C0_id   // I2C module base address

#define DS3231_ADDR   0xD0      // 0b1101000X, X - direction: 0 - write, 1 - read.


// Registers:
enum DS3231_Registers {
  REG_SEC		  = 0x00,
  REG_MIN		  = 0x01,
  REG_HOUR	  = 0x02,
  REG_DOW		  = 0x03,
  REG_DATE	  = 0x04,
  REG_MON		  = 0x05,
  REG_YEAR	  = 0x06,
  REG_CTRL	  = 0x0e,
  REG_STATUS	= 0x0f,
  REG_AGING	  = 0x10,
  REG_TEMPM	  = 0x11,
  REG_TEMPL	  = 0x12
};


/// Read register
uint8_t ds3231_read(uint8_t reg);

/// Read in multi-byte mode (max len 4)
uint32_t ds3231_read_multi(uint8_t first_reg, uint8_t len);

/// Write register
void ds3231_write(uint8_t reg, uint8_t val);

/// Write several registers in multi-byte mode (max len 4)
void ds3231_write_multi(uint8_t first_reg, uint8_t len, uint32_t vals);

#endif // DS3231_H
