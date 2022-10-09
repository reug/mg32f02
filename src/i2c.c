#include "i2c.h"
#include "MG32x02z_CSC.h"
#include "MG32x02z_I2C.h"
#include "core.h"
#include "ulib.h"
#include "utils.h"


#ifdef I2C_DEBUG
const char i2c_flags[24][8]={
  "BUSYF ","EVENT ","BUFF  ","ERRF  ","TMOUT ","WUPF  ","RXF   ","TXF   ",
  "RSTRF ","STOPF ","CNTF  ","ERRCF ","SADRF ","SLAF  ","MSTF  ","RWF   ",
  "TSCF  ","STPSTR","TXRF  ","ROVRF ","TOVRF ","NACKF ","ALOSF ","BERRF "
};

uint32_t i2c_status[16];
#endif


void i2c_init(uint32_t id) {
  RH(CSC_KEY_h0) = 0xA217; // unlock access to CSC regs
#ifdef  MG32F02A032
  RB(CSC_APB0_b1) |= CSC_APB0_I2C0_EN_enable_b1;
#endif
#ifdef  MG32F02A128
  RB(CSC_APB0_b1) |= (id & 0x00010000) ? CSC_APB0_I2C1_EN_enable_b1 : CSC_APB0_I2C0_EN_enable_b1;
#endif
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


void i2c_reset(uint32_t id) {
  RB(id+( I2C0_CR0_b0 -I2C0_Base)) &= ~ I2C_CR0_EN_mask_b0; // выключаем модуль
  __NOP();
  RW(id+( I2C0_STA_w -I2C0_Base)) = 0xffffffff; // сбрасываем все флаги
  RB(id+( I2C0_CR0_b0 -I2C0_Base)) |= I2C_CR0_EN_enable_b0; // включаем модуль
}


void i2c_master_startw(uint32_t id, uint8_t addr) {
  RW(id+( I2C0_CR2_w -I2C0_Base)) =
      (1 << 8) | // BUF_CNT
      I2C_CR2_STA_LCK_un_locked_w | I2C_CR2_STA_mask_w; // STA
  RB(id+( I2C0_DAT_b0 -I2C0_Base)) = addr;
  i2c_setup_tmout(id,0);
  //while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & I2C_STA_TSCF_happened_w)) ; // Работает, но лучше так:
  while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & (I2C_STA_TXF_happened_w | I2C_STA_TMOUTF_happened_w) )) ;
}


void i2c_master_startr(uint32_t id, uint8_t addr) {
  //i2c_status[0]=RW(id+( I2C0_STA_w -I2C0_Base));  i2c_status[1]=RW(id+( I2C0_CR2_w -I2C0_Base));
  RW(id+( I2C0_CR2_w -I2C0_Base)) =
      (1 << 8) | // BUF_CNT
      I2C_CR2_STA_LCK_un_locked_w | I2C_CR2_STA_mask_w; // STA
  //i2c_status[2]=RW(id+( I2C0_STA_w -I2C0_Base));  i2c_status[3]=RW(id+( I2C0_CR2_w -I2C0_Base));
  RB(id+( I2C0_DAT_b0 -I2C0_Base)) = addr | 0x01;
  //i2c_status[4]=RW(id+( I2C0_STA_w -I2C0_Base));  i2c_status[5]=RW(id+( I2C0_CR2_w -I2C0_Base));
  i2c_setup_tmout(id,0);
  //while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & I2C_STA_TSCF_happened_w)) ; // Работает, но лучше так:
  while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & (I2C_STA_SADRF_happened_w | I2C_STA_TMOUTF_happened_w) )) ;
  //while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & I2C_STA_TXF_happened_w)) ; //< НЕ РАБОТАЕТ В РЕЖИМЕ ПРИЕМА!
  //i2c_status[3]=RW(id+( I2C0_CR2_w -I2C0_Base));
}


void i2c_master_stop(uint32_t id) {
  RB(id+( I2C0_CR2_b0 -I2C0_Base)) = I2C_CR2_STO_LCK_un_locked_b0 | I2C_CR2_STO_mask_b0; // STO
}


void i2c_wait_start(uint32_t id) {
  i2c_setup_tmout(id,0);
  while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & (I2C_STA_RSTRF_happened_w | I2C_STA_TMOUTF_happened_w) )) ;
}


void i2c_wait_stop(uint32_t id) {
  i2c_setup_tmout(id,0);
  while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & (I2C_STA_STOPF_happened_w | I2C_STA_TMOUTF_happened_w) )) ;
}


void i2c_master_send(uint32_t id, uint8_t len, uint32_t data) {
  RW(id+( I2C0_STA_w -I2C0_Base)) |= I2C_STA_TXF_mask_w; // сбрасываем TXF, иначе финальная проверка в конце функции может сработать сразу после старта
  RW(id+( I2C0_CR2_w -I2C0_Base)) =
      ((len & 0x07) << 8) | // BUF_CNT
      ((len & I2C_STOP)   ? (I2C_CR2_CMD_TC_enable_w | I2C_CR2_STO_LCK_un_locked_w | I2C_CR2_PSTO_mask_w) : 0) | // auto stop
      ((len & I2C_START)  ? (I2C_CR2_CMD_TC_enable_w | I2C_CR2_STA_LCK_un_locked_w | I2C_CR2_PSTA_mask_w) : 0);  // auto restart
  RW(id+( I2C0_DAT_w -I2C0_Base)) = data;
  i2c_setup_tmout(id,0);
  while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & (I2C_STA_TXF_happened_w | I2C_STA_TMOUTF_happened_w) )) ;
}


uint32_t i2c_master_recv(uint32_t id, uint8_t len) {
  //uint32_t d;

  //i2c_status[6]=RW(id+( I2C0_STA_w -I2C0_Base));  i2c_status[7]=RW(id+( I2C0_CR2_w -I2C0_Base));

  //RW(id+( I2C0_STA_w -I2C0_Base)) |= I2C_STA_RXF_mask_w; // Так сбрасывать RXF не надо! Иначе будет считывание лишних байт
  //d=RW(id+( I2C0_DAT_w -I2C0_Base)); // Вот так RXF сбросить можно!

  //i2c_status[8]=RW(id+( I2C0_STA_w -I2C0_Base));  i2c_status[9]=RW(id+( I2C0_CR2_w -I2C0_Base));

  RW(id+( I2C0_CR2_w -I2C0_Base)) =
      ((len & 0x07) << 8) | // BUF_CNT
      ((len & I2C_ACK)    ? (I2C_CR2_CMD_TC_enable_w | I2C_CR2_AA_LCK_un_locked_w | I2C_CR2_PAA_mask_w) : 0) |  // ACK
      ((len & I2C_STOP)   ? (I2C_CR2_CMD_TC_enable_w | I2C_CR2_STO_LCK_un_locked_w | I2C_CR2_PSTO_mask_w) : 0); // auto stop

  //i2c_status[10]=RW(id+( I2C0_STA_w -I2C0_Base));  i2c_status[11]=RW(id+( I2C0_CR2_w -I2C0_Base));

  i2c_setup_tmout(id,0);

  while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & (I2C_STA_RXF_happened_w | I2C_STA_TMOUTF_happened_w) )) ;
  //i2c_status[12]=RW(id+( I2C0_STA_w -I2C0_Base));  i2c_status[13]=RW(id+( I2C0_CR2_w -I2C0_Base));
  return RW(id+( I2C0_DAT_w -I2C0_Base));

  //i2c_status[14]=RW(id+( I2C0_STA_w -I2C0_Base));  i2c_status[15]=RW(id+( I2C0_CR2_w -I2C0_Base));

  //return d;
}


//void i2c_setup_tmout(uint32_t id, uint8_t mode) {
//
//}


#ifdef I2C_DEBUG
void i2c_print_status(uint32_t status) {
  int i=24;
  do {
    i--;
    if (status & 0x00800000) uart_send(PORT,i2c_flags[i],6);
    else uart_send(PORT,"----- ",6);
    if ((i & 7)==0) uart_puts(PORT,"",UART_NEWLINE_CRLF);
    status <<= 1;
  }
  while (i);
}
#endif
