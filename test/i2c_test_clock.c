#include "i2c_test.h"
#include "i2c.h"
#include "MG32x02z_I2C.h"
#include "core.h"
#include "ulib.h"
#include "utils.h"
#include "ds3231.h"
#include "tm1637.h"


/// Установка времени отдельными командами. Формат: 0xHHMMSS
void time_set_v1(uint32_t t) {
  ds3231_write(REG_SEC, t);
  ds3231_write(REG_MIN, t >> 8);
  ds3231_write(REG_HOUR,t >> 16);
}

/// Установка времени в режиме multi-byte. Формат: 0xHHMMSS
void time_set_v2(uint32_t t) {
  ds3231_write_multi(REG_SEC,3,t);
}

/// Считывание времени отдельными командами
uint32_t time_get_v1() {
  uint8_t d[4];
  d[0]=ds3231_read(REG_SEC);
  d[1]=ds3231_read(REG_MIN);
  d[2]=ds3231_read(REG_HOUR);
  d[3]=0;
  return *(uint32_t*)d;
}

/// Считывание времени в режиме multi-byte
uint32_t time_get_v2() {
  return ds3231_read_multi(REG_SEC,3);
}


void i2c_test_master() {
  uint32_t d;

  //register uint32_t ba=I2C0_Base + (DS3231_PORT ? 0x10000 : 0); // base addr
  i2c_init(DS3231_PORT);
  // Настройка выводов
  //RH(PB_CR10_h0) = (2 << 12) | (1 << 5) | 1; // PB10: I2C0_SCL, pull-up, open drain
  //RH(PB_CR11_h0) = (2 << 12) | (1 << 5) | 1; // PB11: I2C0_SDA, pull-up, open drain
  // Альтернативный вариант:
  RH(PC_CR8_h0) = (2 << 12) | (1 << 5) | 1; // PC8: I2C0_SCL, pull-up, open drain
  RH(PC_CR9_h0) = (2 << 12) | (1 << 5) | 1; // PC9: I2C0_SDA, pull-up, open drain

  //debug32hex('S',RW(DS3231_PORT+( I2C0_STA_w -I2C0_Base)));
  //debug32hex('C',RW(DS3231_PORT+( I2C0_CLK_w -I2C0_Base)));
  //debug32hex('M',RW(DS3231_PORT+( I2C0_CR0_w -I2C0_Base)));
  // Настройка тактирования
  i2c_setup_clock(DS3231_PORT,
      I2C_CLK_TMO_CKS_div64_h0 |  // CK_TMO: F(CK_PSC)/64 = 12500 Hz
      ((15 -1) << I2C_CLK_CK_PSC_shift_h0) | // CK_PSC: 12 MHz /15 = 800 kHz
      I2C_CLK_CK_DIV_div8_h0 |    // CK_I2Cx_INT: 200 kHz => F = 50 kHz
      I2C_CLK_CK_SEL_proc_h0      // I2Cx_CK_SEL: APB, 12 MHz
  );
  // Тайминг режима master
  //RW(I2C0_CR1_w) = 0x0202;
  // Настройка режима работы
  i2c_setup_mode(DS3231_PORT,
      I2C_CR0_PDRV_SEL_1t_w |
      I2C_CR0_BUF_EN_enable_w |   // Режим работы через буфер
      I2C_CR0_MDS_i2c_w           // I2C : Single/Multi-Master/ Slave mode
  );

  tm1637_init(); tm1637_brightness(10);

  while (1) {
    d=time_get_v1();
    if (i2c_get_tmout(DS3231_PORT)) {
      d=i2c_get_status(DS3231_PORT);
      debug32hex('S',d); i2c_print_status(d);
      led2_flash();
      i2c_clr_status(DS3231_PORT, I2C_STA_TMOUTF_mask_w);
      d=0xE001;
    }
    else {
      debug32hex('T',d);
    }
    tm1637_put_hex(d & 0xFFFF,1);
    delay_ms(500);
    tm1637_put_hex(d & 0xFFFF,0);
    delay_ms(500);
  }

  //debug('c',ds3231_read(REG_CTRL));  debug('s',ds3231_read(REG_STATUS));


}


void i2c_test_slave() {
  uint32_t d;

  //register uint32_t ba=I2C0_Base + (DS3231_PORT ? 0x10000 : 0); // base addr
  i2c_init(DS3231_PORT);
  // Настройка выводов
  //RH(PB_CR10_h0) = (2 << 12) | (1 << 5) | 1; // PB10: I2C0_SCL, pull-up, open drain
  //RH(PB_CR11_h0) = (2 << 12) | (1 << 5) | 1; // PB11: I2C0_SDA, pull-up, open drain
  // Альтернативный вариант:
  RH(PC_CR8_h0) = (2 << 12) | (1 << 5) | 1; // PC8: I2C0_SCL, pull-up, open drain
  RH(PC_CR9_h0) = (2 << 12) | (1 << 5) | 1; // PC9: I2C0_SDA, pull-up, open drain

  //debug32hex('S',RW(DS3231_PORT+( I2C0_STA_w -I2C0_Base)));
  //debug32hex('C',RW(DS3231_PORT+( I2C0_CLK_w -I2C0_Base)));
  //debug32hex('M',RW(DS3231_PORT+( I2C0_CR0_w -I2C0_Base)));
  // Настройка тактирования
  i2c_setup_clock(DS3231_PORT,
      I2C_CLK_TMO_CKS_div64_h0 |  // CK_TMO: F(CK_PSC)/64 = 12500 Hz
      ((15 -1) << I2C_CLK_CK_PSC_shift_h0) | // CK_PSC: 12 MHz /15 = 800 kHz
      I2C_CLK_CK_DIV_div8_h0 |    // CK_I2Cx_INT: 200 kHz => F = 50 kHz
      I2C_CLK_CK_SEL_proc_h0      // I2Cx_CK_SEL: APB, 12 MHz
  );
  // Тайминг режима master
  //RW(I2C0_CR1_w) = 0x0202;
  // Настройка режима работы
  i2c_setup_mode(DS3231_PORT,
      I2C_CR0_PDRV_SEL_1t_w |
      I2C_CR0_BUF_EN_enable_w |   // Режим работы через буфер
      I2C_CR0_MDS_i2c_w           // I2C : Single/Multi-Master/ Slave mode
  );

//  RH(I2C0_TMOUT_h0) =
//      (5 << I2C_TMOUT_TMO_CNT_shift_h0) |
//      I2C_TMOUT_TMO_MDS_scl_low_h0 |
//      I2C_TMOUT_TMO_EN_enable_h0;

  //time_set_v2(0x162345);
  //return;
  while (1) {
    d=time_get_v1();
    if (i2c_get_tmout(DS3231_PORT)) {
      d=i2c_get_status(DS3231_PORT);
      debug32hex('S',d); i2c_print_status(d);
      led2_flash();
      i2c_clr_status(DS3231_PORT, I2C_STA_TMOUTF_mask_w);
    }
    else debug32hex('T',d);
    delay_ms(500);
  }

  //debug('c',ds3231_read(REG_CTRL));  debug('s',ds3231_read(REG_STATUS));


}
