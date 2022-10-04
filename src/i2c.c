#include "i2c.h"
#include "MG32x02z__RegAddress.h"
#include "MG32x02z_CSC.h"
#include "MG32x02z_I2C.h"
#include "core.h"
#include "ulib.h"


void i2c_init(uint32_t id) {
  RH(CSC_KEY_h0) = 0xA217; // unlock access to CSC regs
  RB(CSC_APB0_b1) |= (id & 0x00010000) ? CSC_APB0_I2C1_EN_enable_b1 : CSC_APB0_I2C0_EN_enable_b1;
  RH(CSC_KEY_h0) = 0; // lock access to CSC regs
}


void i2c_setup_mode(uint32_t id, uint32_t mode) {
  RW(id+( I2C0_CR0_w -I2C0_Base)) = mode | I2C_CR0_EN_enable_w; // включаем модуль
}


void i2c_setup_clock(uint32_t id, uint16_t mode) {
  RH(id+( I2C0_CLK_h0 -I2C0_Base)) = mode;
}


void i2c_setup_int(uint32_t id, uint32_t flags) {
  RW(id+( I2C0_INT_w -I2C0_Base)) = flags | I2C_INT_IEA_enable_w; // включаем прерывания в модуле
  // включаем прерывание в модуле NVIC:
  RW(CPU_ISER_w) = 1 << ((id & 0x00010000) ? 29 : 28); // SETENA
}


void i2c_master_start(uint32_t id) {
  RW(id+( I2C0_CR2_w -I2C0_Base)) =
      ((1 << 8) & I2C_CR2_BUF_CNT_mask_w) |
      //I2C_CR2_CMD_TC_enable_w |
      I2C_CR2_AA_LCK_un_locked_w | I2C_CR2_AA_mask_w | // AA
      //I2C_CR2_AA_LCK_un_locked_w | I2C_CR2_PAA_mask_w | // AA
      //I2C_CR2_STO_LCK_un_locked_w | I2C_CR2_STO_mask_w |
      //I2C_CR2_STO_LCK_un_locked_w | I2C_CR2_PSTO_mask_w |
      I2C_CR2_STA_LCK_un_locked_w | I2C_CR2_STA_mask_w; // STA
      //I2C_CR2_STA_LCK_un_locked_w | I2C_CR2_PSTA_mask_w; // STA
}


void i2c_master_stop(uint32_t id) {
  RB(id+( I2C0_CR2_b0 -I2C0_Base)) = I2C_CR2_STO_LCK_un_locked_b0 | I2C_CR2_STO_mask_b0; // STO
}


void i2c_master_send(uint32_t id, uint8_t data) {
//  uint8_t d=0,i,b;
//  for (i=0; i<8; i++) {
//    b = ((data << i) & 0x80) ? 1:0;
//    d |= (b << i);
//  }
  RW(id+( I2C0_DAT_w -I2C0_Base)) = data; // & 0xfe;
  while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & I2C_STA_TSCF_happened_w)) ;
  //while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & I2C_STA_TXF_happened_w)) ;
  //while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & I2C_STA_RSTRF_happened_w)) ;
}
