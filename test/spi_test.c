#include "spi_test.h"
#include "core.h"
#include "MG32x02z_GPIO.h"
#include "MG32x02z_SPI.h"
#include "MG32x02z_EXIC.h"
#include "hwcf.h"
#include "ulib.h"
#include "utils.h"
#include "ic/enc28j60.h"
#include "ethernet.h"


/// Local MAC address
const uint8_t addr[6]={0x02,0xEE,0x10,0x00,0x00,0x01};

/// Destination MAC address
const uint8_t dest[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


void pin_test(uint8_t p) {
  RH(PA_CR8_h0) = 2;  RH(PA_CR9_h0) = 2;  RH(PA_CR10_h0) = 2;  RH(PA_CR11_h0) = 2;
  while (1) {
    RH(PA_SC_h0) = (1 << p);  delay_ms(1);
    RH(PA_SC_h1) = (1 << p);  delay_ms(1);
  }
}


/// Обработчик внешнего прерывания
void exint0_hdl() {
  uint16_t n;
  led1_on();
  //debug('I',RH(EXIC_PA_PF_h0));
  enc28j60_bfc(EIE,EIE_INTIE); // сбрасываем INTIE бит согласно даташиту

  n=eth_recvpkt(eth_frame,ETH_FRAME_MAXSIZE);
  if (n) {
    debug('L',n);
    debugbuf(eth_frame,n);
  }

  enc28j60_bfs(EIE,EIE_INTIE); // устанавливаем INTIE бит согласно даташиту

  RH(EXIC_PA_PF_h0) = HW_EXINT0_MASK; // сбрасываем флаг (EXIC_PA_PF_PA12_PF_mask_h0)
  led1_off();
}


/// Настройка внешнего прерывания EXIC
void exint_setup() {
  // Настройка вывода и контроллера EXIC
  RH(HW_EXINT0_CRH0) =
    //PX_CR_INV_enable_h0 | // инверсия не нужна, активный уровень низкий по-умолчанию!
    PX_CR_PU_enable_h0 |  // подтяжка
    PX_CR_IOM_din_h0;     // цифровой вход
  //RW(EXIC_PA_TRGS_w) |= (1 << (HW_EXINT0_BIT*2)); // Level
  RW(EXIC_PA_TRGS_w) |= (2 << (HW_EXINT0_BIT*2)); // Edge
  RW(EXIC_PA_MSK_w) |= (1 << HW_EXINT0_BIT); // OR (AOM
  RB(EXIC_INT_b0) |= EXIC_INT_PA_IEA_enable_b0;
  // Установка обработчика прерывания:
  SVC2(SVC_HANDLER_SET,3,exint0_hdl);
  RW(CPU_ISER_w) = (1 << 3); // SETENA 3
}


/// Буфер данных
#define BUFLEN 1536
uint8_t  buf[BUFLEN]; ///< данные буфера
uint32_t bufp;        ///< указатель буфера
uint32_t bufn;        ///< размер данных на отправку


void spi_test_master() {
  uint16_t n;
  uint32_t d;
  char s[8];
  //pin_test(8);

  // Настройка выводов:
  HW_SPI0_SETMISO;  HW_SPI0_SETMOSI;  HW_SPI0_SETSCK;  HW_SPI0_SETNSS;

  // Инициализация, настройка тактирования:
  spi_init();

  // Настройка режима работы
  spi_setup_mode(
    //SPI_NSS_PEN |
    SPI_NSSO_INV | SPI_NSS_SWEN | // software NSS control
    SPI_NSSO_EN | SPI_MASTER | SPI_MSB | SPI_CPHA_LE | SPI_CPOL_LOW
    | SPI_CR0_DOUT_MDS_enable_w // надо включить, если нет резистора подтяжки
  );

  RB(SPI0_CR2_b1) = 1; // SPI0_RX_TH = 1
  RW(SPI0_CR2_w) |= (8 << SPI_CR2_DSIZE_shift_w); // Размер кадра в битах
  spi_flush_rx();

  eth_init(addr);
  uart_puts(PORT,"EREVID: ",UART_NEWLINE_NONE); strUint16hex(s,enc28j60_rcr(EREVID)); uart_puts(PORT,s,UART_NEWLINE_CRLF);
  uart_puts(PORT,"PHID1:  ",UART_NEWLINE_NONE); strUint16hex(s,enc28j60_rcr(PHID1)); uart_puts(PORT,s,UART_NEWLINE_CRLF);
  uart_puts(PORT,"PHID2:  ",UART_NEWLINE_NONE); strUint16hex(s,enc28j60_rcr(PHID2)); uart_puts(PORT,s,UART_NEWLINE_CRLF);
  uart_puts(PORT,"ERXFCON:",UART_NEWLINE_NONE); strUint16hex(s,enc28j60_rcr(ERXFCON)); uart_puts(PORT,s,UART_NEWLINE_CRLF);

  exint_setup();
  eth_setup_int();
  eth_clear_int();

  //spi_nss(1);  spi_tx(0xff);  spi_nss(0);  delay_ms(100);
  uart_puts(PORT,"MAC: ",UART_NEWLINE_NONE); eth_get_addr(s); debugbuf(s,6);

  while (1) {
    //enc28j60_bfs(ECON2, ECON2_PKTDEC);

//  n=eth_recvpkt(eth_frame,ETH_FRAME_MAXSIZE);
//  if (n) {
//    debug('L',n);
//    debugbuf(eth_frame,n);
//  }
//  eth_clear_int();


    delay_ms(1000);
  }

}

