// DS3231 library for MG32F02 MCUs (ARM Cortex-M0)
// by reug@mail.ru, 2022

#include "ds3231.h"

// https://github.com/NorthernWidget/DS3231
// http://www.rinkydinkelectronics.com/library.php?id=73


uint8_t ds3231_read(uint8_t reg) {
  uint32_t d;
  i2c_master_startw(DS3231_PORT, DS3231_ADDR);
  i2c_master_send(DS3231_PORT, 1 | I2C_START, reg);
  i2c_wait_start(DS3231_PORT);

  i2c_master_startr(DS3231_PORT, DS3231_ADDR);
  d=i2c_master_recv(DS3231_PORT, 1 | I2C_STOP);
  i2c_wait_stop(DS3231_PORT);
  return d;
}


void ds3231_write(uint8_t reg, uint8_t val) {
  i2c_master_startw(DS3231_PORT, DS3231_ADDR);
  i2c_master_send(DS3231_PORT, 2 | I2C_STOP, ((uint32_t)val << 8) | reg );
  i2c_wait_stop(DS3231_PORT);
}


void ds3231_write_multi(uint8_t first_reg, uint8_t len, uint32_t vals) {
  i2c_master_startw(DS3231_PORT, DS3231_ADDR);
  i2c_master_send(DS3231_PORT,1,first_reg);
  i2c_master_send(DS3231_PORT, (len & 0x07) | I2C_STOP, vals);
  i2c_wait_stop(DS3231_PORT);
}
