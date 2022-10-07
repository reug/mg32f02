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
  //int i;
  uint32_t d;
  //ds[0]=i2c_get_status(DS3231_PORT);
  i2c_master_startw(DS3231_PORT, DS3231_ADDR); TMOUT_CHECK2
  //ds[1]=i2c_get_status(DS3231_PORT);
  i2c_master_send(DS3231_PORT, 1 | I2C_START, reg); TMOUT_CHECK2
  //ds[2]=i2c_get_status(DS3231_PORT);
  i2c_wait_start(DS3231_PORT); TMOUT_CHECK2
  //ds[3]=i2c_get_status(DS3231_PORT);

  i2c_master_startr(DS3231_PORT, DS3231_ADDR); TMOUT_CHECK2
  //ds[4]=i2c_get_status(DS3231_PORT);
  d=i2c_master_recv(DS3231_PORT, 1 | I2C_STOP); TMOUT_CHECK2
  //ds[5]=i2c_get_status(DS3231_PORT);
  i2c_wait_stop(DS3231_PORT); TMOUT_CHECK2
  //ds[6]=i2c_get_status(DS3231_PORT);
//#ifdef I2C_DEBUG
//  for (i=0; i<16; i++) {
//    debug32hex('0'+i,i2c_status[i]);
//    if ((i & 1)==0) i2c_print_status(i2c_status[i]);
//  }
//#endif
  return d;
failure:
  return 0;
}


uint32_t ds3231_read_multi(uint8_t first_reg, uint8_t len) {
  uint32_t d;
  i2c_master_startw(DS3231_PORT, DS3231_ADDR); TMOUT_CHECK2
  i2c_master_send(DS3231_PORT, 1 | I2C_START, first_reg); TMOUT_CHECK2
  i2c_wait_start(DS3231_PORT); TMOUT_CHECK2
  i2c_master_startr(DS3231_PORT, DS3231_ADDR); TMOUT_CHECK2
  d=i2c_master_recv(DS3231_PORT, (len & 0x07) | I2C_STOP); TMOUT_CHECK2
  i2c_wait_stop(DS3231_PORT); TMOUT_CHECK2
  return d;
failure:
  return 0;
}


void ds3231_write(uint8_t reg, uint8_t val) {
  i2c_master_startw(DS3231_PORT, DS3231_ADDR); TMOUT_CHECK
  i2c_master_send(DS3231_PORT, 2 | I2C_STOP, ((uint32_t)val << 8) | reg ); TMOUT_CHECK
  i2c_wait_stop(DS3231_PORT); TMOUT_CHECK
}


void ds3231_write_multi(uint8_t first_reg, uint8_t len, uint32_t vals) {
  i2c_master_startw(DS3231_PORT, DS3231_ADDR); TMOUT_CHECK
  i2c_master_send(DS3231_PORT,1,first_reg); TMOUT_CHECK
  i2c_master_send(DS3231_PORT, (len & 0x07) | I2C_STOP, vals); TMOUT_CHECK
  i2c_wait_stop(DS3231_PORT); TMOUT_CHECK
}
