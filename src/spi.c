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
    SPI_CLK_CK_DIV_div2_h0  |         // F(DIV=2)   ->  2 MHz = F(CK_SPI0_INT)
    //SPI_CLK_CK_DIV_div4_h0  |         // F(DIV=4)   ->  1 MHz = F(CK_SPI0_INT)
    //SPI_CLK_CK_DIV_div8_h0  |         // F(DIV=8)   ->  0.5 MHz = F(CK_SPI0_INT)
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


uint8_t spi_tx(uint8_t data) {
/*
  // Рабочий вариант, но не оптимальный
  RB(SPI0_TDAT_b0) = data;
  //while (! (RB(SPI0_STA_b0) & SPI_STA_TCF_happened_b0)) ;
  while (! (RB(SPI0_STA_b0) & SPI_STA_RXF_happened_b0)) ;
  data = RB(SPI0_RDAT_b0);  // фиктивное чтение
*/
  // Оптимизированный вариант
  asm(
    "mov  r1,%[RDAT]\n"
    "mov  r2,%[BASE]\n"
    "lsl  r2,r2,#24\n"              // r2 <- SPI0_Base
    "add  r3,r2,r1\n"               // r3 <- SPI0_RDAT_w
    "strb r0,[r3, %[TDAT]]\n"       // RB(SPI0_TDAT_b0) = data;
    "mov  r1,%[RXF_MASK]\n"         // r1 <- SPI_STA_RXF_mask_b0
    "spi_tx_loop:\n"
    "ldrb r0,[r2, %[STA]]\n"
    "tst  r0,r1\n"                  // RB(SPI0_STA_b0) & SPI_STA_RXF_happened_b0 ?
    "beq  spi_tx_loop\n"
    "ldrb r0,[r3, #0]\n"            // data = RB(SPI0_RDAT_b0);  // фиктивное чтение
    :: [BASE]"I"(SPI0_Base >> 24),
       [STA]"I"(SPI0_STA_w-SPI0_Base),
       [RDAT]"I"(SPI0_RDAT_w-SPI0_Base),
       [TDAT]"I"(SPI0_TDAT_w-SPI0_RDAT_w),
       [RXF_MASK]"I"(SPI_STA_RXF_mask_b0)
    : "r0","r1","r2","r3"
  );
}

/*
uint8_t spi_rx() {
  RB(SPI0_TDAT_b0) = 0xFF;
  while (! (RB(SPI0_STA_b0) & SPI_STA_RXF_happened_b0)) ;
  //while ( (RB(SPI0_STA_b2) & SPI_STA_RX_LVL_mask_b2) ==0) ;
  return RB(SPI0_RDAT_b0);
}
*/


void spi_slave_tx(uint8_t data) {
  RB(SPI0_TDAT_b0) = data;
  while (! (RB(SPI0_STA_b0) & SPI_STA_TXF_happened_b0)) ;
}


uint8_t spi_slave_rx() {
  while (! (RB(SPI0_STA_b0) & SPI_STA_RXF_happened_b0)) ;
  //while ( (RB(SPI0_STA_b2) & SPI_STA_RX_LVL_mask_b2) ==0) ;
  return RB(SPI0_RDAT_b0);
}

//
//void spi_send(uint32_t opts, uint8_t len, uint32_t data) {
//  RW(SPI0_TDAT_w) = data;
//  while (! (RW(SPI0_STA_w) & SPI_STA_TXF_happened_w)) ;
//  while (! (RW(SPI0_STA_w) & SPI_STA_TCF_happened_w)) ;
//}
//
//
//uint32_t spi_recv(uint32_t opts, uint8_t len) {
//  while (! (RW(SPI0_STA_w) & SPI_STA_RXF_happened_w)) ;
//  return RW(SPI0_RDAT_w);
//}

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
