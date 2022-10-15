// DS3231 library for MG32F02 MCUs (ARM Cortex-M0)
// by reug@mail.ru, 2022

#include "ds3231.h"

// https://github.com/NorthernWidget/DS3231
// http://www.rinkydinkelectronics.com/library.php?id=73


// For debug only:
#include "utils.h"

#define TMOUT_CHECK     if (i2c_get_tmout(DS3231_PORT)) return;
#define TMOUT_CHECK2    if (i2c_get_tmout(DS3231_PORT)) goto failure;


uint8_t ds3231_read(uint8_t reg) {
  uint32_t d;
  i2c_master_startw(DS3231_PORT, DS3231_ADDR); TMOUT_CHECK2
  i2c_master_send(DS3231_PORT, I2C_START, 1, reg); TMOUT_CHECK2
  i2c_wait_start(DS3231_PORT); TMOUT_CHECK2

  i2c_master_startr(DS3231_PORT, DS3231_ADDR); TMOUT_CHECK2
  d=i2c_master_recv(DS3231_PORT, I2C_STOP, 1); TMOUT_CHECK2
  i2c_wait_stop(DS3231_PORT); TMOUT_CHECK2
  return d;
failure:
  return 0;
}


uint32_t ds3231_read_multi(uint8_t first_reg, uint8_t len) {
  uint32_t d;
  i2c_master_startw(DS3231_PORT, DS3231_ADDR); TMOUT_CHECK2
  i2c_master_send(DS3231_PORT, I2C_START, 1, first_reg); TMOUT_CHECK2
  i2c_wait_start(DS3231_PORT); TMOUT_CHECK2
  i2c_master_startr(DS3231_PORT, DS3231_ADDR); TMOUT_CHECK2
  d=i2c_master_recv(DS3231_PORT, I2C_STOP, len & 0x07); TMOUT_CHECK2
  i2c_wait_stop(DS3231_PORT); TMOUT_CHECK2
  return d;
failure:
  return 0;
}


void ds3231_write(uint8_t reg, uint8_t val) {
  i2c_master_startw(DS3231_PORT, DS3231_ADDR); TMOUT_CHECK
  i2c_master_send(DS3231_PORT, I2C_STOP, 2, ((uint32_t)val << 8) | reg ); TMOUT_CHECK
  i2c_wait_stop(DS3231_PORT); TMOUT_CHECK
}


void ds3231_write_multi(uint8_t first_reg, uint8_t len, uint32_t vals) {
  i2c_master_startw(DS3231_PORT, DS3231_ADDR); TMOUT_CHECK
  i2c_master_send(DS3231_PORT, I2C_NOOPTS, 1, first_reg); TMOUT_CHECK
  i2c_master_send(DS3231_PORT, I2C_STOP, len & 0x07, vals); TMOUT_CHECK
  i2c_wait_stop(DS3231_PORT); TMOUT_CHECK
}


#ifdef DS3231_ONEBYTE_MODE
// Установка времени отдельными командами
void clock_set_bcd(uint32_t t) {
  ds3231_write(REG_SEC, t);
  ds3231_write(REG_MIN, t >> 8);
  ds3231_write(REG_HOUR,t >> 16);
}

#else
// Установка времени в режиме multi-byte
void clock_set_bcd(uint32_t t) {
  ds3231_write_multi(REG_SEC,3,t);
}
#endif

#ifdef DS3231_ONEBYTE_MODE
// Считывание времени отдельными командами
uint32_t clock_get_bcd() {
  uint8_t d[4];
  d[0]=ds3231_read(REG_SEC);
  d[1]=ds3231_read(REG_MIN);
  d[2]=ds3231_read(REG_HOUR);
  d[3]=0;
  return *(uint32_t*)d;
}

#else
// Считывание времени в режиме multi-byte
uint32_t clock_get_bcd() {
  return ds3231_read_multi(REG_SEC,3);
}
#endif
