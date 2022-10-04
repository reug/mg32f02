#include "i2c_test.h"
#include "i2c.h"
#include "MG32x02z__RegAddress.h"
#include "MG32x02z_I2C.h"
#include "core.h"
#include "ulib.h"
#include "tm1637.h"


void i2c_test_master() {
  //register uint32_t ba=I2C0_Base + (I2C_PORT ? 0x10000 : 0); // base addr
  i2c_init(I2C_PORT);
  // Настройка выводов
  //RH(PB_CR10_h0) = (2 << 12) | (1 << 5) | 1; // PB10: I2C0_SCL, pull-up, open drain
  //RH(PB_CR11_h0) = (2 << 12) | (1 << 5) | 1; // PB11: I2C0_SDA, pull-up, open drain
  // Альтернативный вариант:
  RH(PC_CR8_h0) = (2 << 12) | (1 << 5) | 1; // PC8: I2C0_SCL, pull-up, open drain
  RH(PC_CR9_h0) = (2 << 12) | (1 << 5) | 1; // PC9: I2C0_SDA, pull-up, open drain
  // Настройка тактирования
  i2c_setup_clock(I2C_PORT,
      I2C_CLK_TMO_CKS_div64_h0 |  // CK_TMO: F(CK_PSC)/64 = 12500 Hz
      ((15 -1) << I2C_CLK_CK_PSC_shift_h0) | // CK_PSC: 12 MHz /15 = 800 kHz
      I2C_CLK_CK_DIV_div8_h0 |    // CK_I2Cx_INT: 200 kHz => F = 50 kHz
      I2C_CLK_CK_SEL_proc_h0      // I2Cx_CK_SEL: APB, 12 MHz
  );
  // Тайминг режима master
  //RW(I2C0_CR1_w) = 0x0202;
  // Настройка режима работы
  i2c_setup_mode(I2C_PORT,
      I2C_CR0_PDRV_SEL_1t_w |
      I2C_CR0_BUF_EN_enable_w |   // Режим работы через буфер
      I2C_CR0_MDS_i2c_w           // I2C : Single/Multi-Master/ Slave mode
  );

//  RH(I2C0_TMOUT_h0) =
//      (5 << I2C_TMOUT_TMO_CNT_shift_h0) |
//      I2C_TMOUT_TMO_MDS_scl_low_h0 |
//      I2C_TMOUT_TMO_EN_enable_h0;

  i2c_master_start(I2C_PORT);
  i2c_master_send(I2C_PORT,TM1637_CMD_WDAA);
  i2c_master_stop(I2C_PORT);

  i2c_master_start(I2C_PORT);
  i2c_master_send(I2C_PORT,TM1637_CMD_ADDR | 0x2);
  i2c_master_send(I2C_PORT,0x55); // i2c_master_send(I2C_PORT,0xF);
  i2c_master_send(I2C_PORT,0x55); // i2c_master_send(I2C_PORT,0xF);
  //i2c_master_send(I2C_PORT,0xFF); // i2c_master_send(I2C_PORT,0xF);
  //i2c_master_send(I2C_PORT,0xFF); // i2c_master_send(I2C_PORT,0xF);
  i2c_master_stop(I2C_PORT);

  i2c_master_start(I2C_PORT);
  i2c_master_send(I2C_PORT,TM1637_CMD_CTRL | 0x7);
  i2c_master_stop(I2C_PORT);
  //delay_ms(10);
  //i2c_master_send(I2C_PORT,0x55);
  //delay_ms(10);
  //i2c_master_send(I2C_PORT,0xAA,1);

  //while (! (RW(I2C0_STA_w) & I2C_STA_TXF_happened_w)) ;

}
