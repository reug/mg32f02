#include "i2c_test.h"
#include "i2c.h"
#include "MG32x02z__RegAddress.h"
#include "MG32x02z_I2C.h"
#include "core.h"
#include "ulib.h"
#include "utils.h"
#include "ds3231.h"


void time_set() {
  // Вариант 1: отдельные команды:
  //ds3231_write(REG_HOUR,0x15);  ds3231_write(REG_MIN,0x39); ds3231_write(REG_SEC,0x45); // не работает пока
  // Вариант 2: запись нескольких регистров в режиме multi-byte:
  ds3231_write_multi(REG_SEC,3,0x154320); // 0xHHMMSS
}


void time_get() {
  uint16_t d;
  d=ds3231_read(REG_HOUR); debug('H',d);
  d=ds3231_read(REG_MIN);  debug('M',d);
  d=ds3231_read(REG_SEC);  debug('S',d);
}


void i2c_test_master() {

  //register uint32_t ba=I2C0_Base + (DS3231_PORT ? 0x10000 : 0); // base addr
  i2c_init(DS3231_PORT);
  // Настройка выводов
  //RH(PB_CR10_h0) = (2 << 12) | (1 << 5) | 1; // PB10: I2C0_SCL, pull-up, open drain
  //RH(PB_CR11_h0) = (2 << 12) | (1 << 5) | 1; // PB11: I2C0_SDA, pull-up, open drain
  // Альтернативный вариант:
  RH(PC_CR8_h0) = (2 << 12) | (1 << 5) | 1; // PC8: I2C0_SCL, pull-up, open drain
  RH(PC_CR9_h0) = (2 << 12) | (1 << 5) | 1; // PC9: I2C0_SDA, pull-up, open drain
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

  //time_set(); return;

  while (1) {time_get(); delay_ms(1000); }

  //debug('c',ds3231_read(REG_CTRL));  debug('s',ds3231_read(REG_STATUS));

}
