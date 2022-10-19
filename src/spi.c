#include "spi.h"
#include "MG32x02z_CSC.h"
#include "ulib.h"
#include "utils.h"

/*
#ifdef I2C_DEBUG
const char i2c_flags[24][8]={
  "BUSYF ","EVENT ","BUFF  ","ERRF  ","TMOUT ","WUPF  ","RXF   ","TXF   ",
  "RSTRF ","STOPF ","CNTF  ","ERRCF ","SADRF ","SLAF  ","MSTF  ","RWF   ",
  "TSCF  ","STPSTR","TXRF  ","ROVRF ","TOVRF ","NACKF ","ALOSF ","BERRF "
};

uint32_t i2c_status[16];
#endif
*/

void spi_init() {
  RH(CSC_KEY_h0) = 0xA217; // unlock access to CSC regs
  RB(CSC_APB0_b1) |= CSC_APB0_SPI0_EN_enable_b1;
  RH(CSC_KEY_h0) = 0; // lock access to CSC regs
  // Настройка тактирования
  RH(SPI0_CLK_h0) =
    SPI_CLK_CK_PDIV_div1_h0 |         // F(PDIV=1)  -> 12 MHz
    (2 << SPI_CLK_CK_PSC_shift_h0) |  // F(PSC=3)   ->  4 MHz
    SPI_CLK_CK_DIV_div4_h0  |         // F(DIV=4)   ->  1 MHz = F(CK_SPI0_INT)
    SPI_CLK_CK_SEL_proc_h0;           // CK_SPIx_PR =  12 MHz
}

/*
void i2c_setup_mode(uint32_t id, uint32_t mode) {
  RW(id+( I2C0_CR0_w -I2C0_Base)) = mode | I2C_CR0_EN_enable_w; // включаем модуль
}
*/
/*
void i2c_setup_clock(uint32_t id, uint16_t mode) {
  RH(id+( I2C0_CLK_h0 -I2C0_Base)) = mode;
}
*/

void spi_setup_int(uint16_t flags) {
  RH(SPI0_INT_h0) = flags | SPI_INT_IEA_enable_w; // включаем прерывания в модуле
  // включаем прерывание в модуле NVIC:
  RW(CPU_ISER_w) = 1 << 24; // SETENA
}


void spi_send(uint32_t opts, uint8_t len, uint32_t data) {
//  RW(id+( I2C0_STA_w -I2C0_Base)) |= I2C_STA_TXF_mask_w; // сбрасываем TXF, иначе финальная проверка в конце функции может сработать сразу после старта
//  RW(id+( I2C0_CR2_w -I2C0_Base)) = opts | (len << 8); // BUF_CNT
//  RW(id+( I2C0_DAT_w -I2C0_Base)) = data;
//  i2c_setup_tmout(id,0);
//  while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & (I2C_STA_TXF_happened_w | I2C_STA_TMOUTF_happened_w) )) ;
}


uint32_t spi_recv(uint32_t opts, uint8_t len) {
  //RW(id+( I2C0_STA_w -I2C0_Base)) |= I2C_STA_RXF_mask_w; // Так сбрасывать RXF не надо! Иначе будет считывание лишних байт
  //d=RW(id+( I2C0_DAT_w -I2C0_Base)); // Вот так RXF сбросить можно!

//  RW(id+( I2C0_CR2_w -I2C0_Base)) = opts | (len << 8); // BUF_CNT
//  i2c_setup_tmout(id,0);
//  while (! (RW(id+( I2C0_STA_w -I2C0_Base)) & (I2C_STA_RXF_happened_w | I2C_STA_TMOUTF_happened_w) )) ;
//  return RW(id+( I2C0_DAT_w -I2C0_Base));
}

/*
uint32_t i2c_writebuf(uint32_t id, const void* buf, uint32_t* p, uint32_t len) {
  uint8_t m;
  int32_t n;
  n = len - *p;
  if (n > 0) {
    m = n < 4 ? n : 4;
    RB(id+( I2C0_CR2_b1 -I2C0_Base)) = (m & I2C_CR2_BUF_CNT_mask_b1); // BUF_CNT
    switch (m) {
      case 1: RB(id+( I2C0_DAT_b0 -I2C0_Base)) = *((uint8_t*)buf + *p); break;
      case 2: RH(id+( I2C0_DAT_h0 -I2C0_Base)) = *(uint16_t*)((uint8_t*)buf + *p); break;
      default:
        RW(id+( I2C0_DAT_w -I2C0_Base)) = *(uint32_t*)((uint8_t*)buf + *p); break;
    }
    *p += m;
  }
  return len-*p;
}
*/
/*
uint8_t i2c_readbuf(uint32_t id, void* buf, uint32_t* p) {
  uint8_t n = RB(id+( I2C0_CR2_b1 -I2C0_Base)) & I2C_CR2_BUF_CNT_mask_b1; // BUF_CNT
  *(uint32_t*)((uint8_t*)buf + *p) = RW(id+( I2C0_DAT_w -I2C0_Base));
  *p += n;
  return n;
}
*/
/*
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
*/
