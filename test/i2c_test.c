#include "i2c_test.h"
#include "i2c.h"
#include "core.h"
#include "MG32x02z_I2C.h"
#include "hwcf.h"
#include "ulib.h"
#include "utils.h"
#include "ic/ds3231.h"


void i2c_test_master_setup() {
  i2c_init(DS3231_PORT);
  HW_I2C0_SETSCL;
  HW_I2C0_SETSDA;

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
}


void i2c_test_master_w1r() {
  uint32_t d;

  while (1) {
    d=ds3231_read_multi(6,6); //d=clock_get_bcd();
    if (i2c_get_tmout(DS3231_PORT)) {
      d=i2c_get_status(DS3231_PORT);
      debug32hex('S',d); i2c_print_status(d);
      led2_flash();
      i2c_clr_status(DS3231_PORT, I2C_STA_TMOUTF_mask_w);
    }
    else {
      debug32hex('T',d);
    }
    delay_ms(1000);
  }

}


void i2c_test_master_wN(uint8_t n) {
  uint32_t d;
  uint8_t buf[]={0xA1,0xB2,0xC3,0xD4,0xE5,0xF6};

  while (1) {
    ds3231_write_multi(1,n,*(uint32_t*)buf); //d=clock_get_bcd();
    if (i2c_get_tmout(DS3231_PORT)) {
      d=i2c_get_status(DS3231_PORT);
      debug32hex('S',d); i2c_print_status(d);
      led2_flash();
      i2c_clr_status(DS3231_PORT, I2C_STA_TMOUTF_mask_w);
    }
    else {
      uart_puts(PORT,"OK",UART_NEWLINE_CRLF);
    }
    delay_ms(1000);
  }

}

