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


void i2c_master_startw(uint32_t id, uint8_t addr) {
  RW(id+( I2C0_CR2_w -I2C0_Base)) =
      (1 << 8) | // BUF_CNT
      I2C_CR2_STA_LCK_un_locked_w | I2C_CR2_STA_mask_w; // STA
  RB(id+( I2C0_DAT_b0 -I2C0_Base)) = addr;
  //while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & I2C_STA_TSCF_happened_w)) ; // Работает, но лучше так:
  while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & I2C_STA_TXF_happened_w)) ;
}


void i2c_master_startr(uint32_t id, uint8_t addr) {
  RW(id+( I2C0_CR2_w -I2C0_Base)) =
      (1 << 8) | // BUF_CNT
      I2C_CR2_STA_LCK_un_locked_w | I2C_CR2_STA_mask_w; // STA
  RB(id+( I2C0_DAT_b0 -I2C0_Base)) = addr | 0x01;
  //while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & I2C_STA_TSCF_happened_w)) ; // Работает, но лучше так:
  while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & I2C_STA_SADRF_happened_w)) ;
  //while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & I2C_STA_TXF_happened_w)) ; //< НЕ РАБОТАЕТ В РЕЖИМЕ ПРИЕМА!
}


void i2c_master_stop(uint32_t id) {
  RB(id+( I2C0_CR2_b0 -I2C0_Base)) = I2C_CR2_STO_LCK_un_locked_b0 | I2C_CR2_STO_mask_b0; // STO
}


void i2c_wait_start(uint32_t id) {
  while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & I2C_STA_RSTRF_happened_w)) ;
}


void i2c_wait_stop(uint32_t id) {
  while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & I2C_STA_STOPF_happened_w)) ;
}


void i2c_master_send(uint32_t id, uint8_t len, uint32_t data) {
  RW(id+( I2C0_STA_w -I2C0_Base)) |= I2C_STA_TXF_mask_w; // сбрасываем TXF, иначе финальная проверка в конце функции может сработать сразу после старта
  RW(id+( I2C0_CR2_w -I2C0_Base)) =
      ((len & 0x07) << 8) | // BUF_CNT
      ((len & I2C_STOP)   ? (I2C_CR2_CMD_TC_enable_w | I2C_CR2_STO_LCK_un_locked_w | I2C_CR2_PSTO_mask_w) : 0) | // auto stop
      ((len & I2C_START)  ? (I2C_CR2_CMD_TC_enable_w | I2C_CR2_STA_LCK_un_locked_w | I2C_CR2_PSTA_mask_w) : 0);  // auto restart
  RW(id+( I2C0_DAT_w -I2C0_Base)) = data;
  //while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & I2C_STA_TSCF_happened_w)) ;
  while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & I2C_STA_TXF_happened_w)) ;
}


uint32_t i2c_master_recv(uint32_t id, uint8_t len) {
  //RW(id+( I2C0_STA_w -I2C0_Base)) |= I2C_STA_RXF_mask_w; // сбрасываем RXF на всякий случай
  RW(id+( I2C0_CR2_w -I2C0_Base)) =
      ((len & 0x07) << 8) | // BUF_CNT
      ((len & I2C_ACK)    ? (I2C_CR2_CMD_TC_enable_w | I2C_CR2_AA_LCK_un_locked_w | I2C_CR2_PAA_mask_w) : 0) |  // ACK
      ((len & I2C_STOP)   ? (I2C_CR2_CMD_TC_enable_w | I2C_CR2_STO_LCK_un_locked_w | I2C_CR2_PSTO_mask_w) : 0); // auto stop
  while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & I2C_STA_RXF_happened_w)) ;
  return RW(id+( I2C0_DAT_w -I2C0_Base));
}
