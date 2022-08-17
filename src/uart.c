#include "uart.h"
#include "MG32x02z__RegAddress.h"


void uart_init(uint8_t port_no) {
  register uint32_t da=(uint32_t)port_no*0x10000;

  // Clock source:
  *((volatile uint16_t*)CSC_KEY_h0) = 0xA217; // unlock access to CSC regs
  // (0x4C010022)
  *(volatile uint16_t*)CSC_APB0_h1 = (1 << port_no); // CSC_URTx_EN = 1
   // Leave default APB clock settings: CSC_URTx_CKS (bit 16) = CK_APB (0)
  //*(volatile uint32_t*)CSC_CKS1_w = 0;
  *((volatile uint16_t*)CSC_KEY_h0) = 0x1111; // lock access to CSC regs

  // UART Global Enable (0x52000010)
  *(volatile uint32_t*)(URT0_CR0_w+da) = 1; // URTx_EN = 1

  // UART Clock: (default is used)
  // *(volatile uint32_t*)URT0_CLK_w = 0;
  // URT0_RX_CKS = 0 (UART internal clock source CK_URTx_INT)
  // URT0_TX_CKS = 0 (UART internal clock source CK_URTx_INT)
//      (0 << 1) | // URT0_CK_SEL: PROC : CK_URTx_PR process clock from CSC
//      // URTx_ECK_CKS

  // Frame format (0x52000014)
  // (This register is written no effect if URTx_TX_EN set 1.)
  *(volatile uint32_t*)(URT0_CR1_w+da) =
      (3 << 24) | // URT0_TXOS_NUM --- TX data oversampling samples select. The valid value is from 3 to 31 for oversampling samples from 4 to 32.
      (3 << 8) | // URT0_RXOS_NUM --- RX data oversampling samples select. The valid value is from 3 to 31 for oversampling samples from 4 to 32.
      (1 << 22) | (0 << 18) | (0 << 17) | (1 << 6); // tx_stopbit=1, parity=off, 8 bits, rx_stopbit=1

  // Baud (assume CK_URT0 = 12 MHz ) (0x52000024)
  //
  *(volatile uint16_t*)(URT0_RLR_w+da) =
      (1 << 8) | // URT0_PSR
       12; // URT0_RLP

  // Result: 115200 baud

  // Включаем Baud-Rate Generator, поскольку используем тактирование от него (CK_URTx_INT)
  *(volatile uint32_t*)(URT0_CLK_w+da) = (1 << 24); // URT0_BR_EN = 1

  // Settings (0x52000018)
  *(volatile uint32_t*)(URT0_CR2_w+da) = (1 << 3) | (1 << 2) ; // URT0_TX_EN=1 , URT0_RX_EN=1

}


void uart_tx(uint8_t port_no, uint8_t d) {
  register uint32_t da=(uint32_t)port_no*0x10000;
  while ( (*(volatile uint8_t*)(URT0_STA2_b3+da) & 0x70)); // ждем, пока URT0_TX_LVL != 0
  *(volatile uint8_t*)(URT0_TDAT_b0+da) = d;   // отправляем байт (0x52000034) <= d
}


void uart_send(uint8_t port_no, void* buf, uint32_t len) {
  uint32_t i;
  for (i=0; i<len; i++) uart_tx(port_no, *((uint8_t*)buf+i));
}


void uart_puts(uint8_t port_no, const char* s, uint32_t newline) {
  uint32_t i;
  uint8_t b;
  for (i=0; s[i]!=0; i++) {
    uart_tx(port_no, s[i]);
  }
  for (i=2; i; i--) {
    b=(newline & 0xFF);
    if (b) uart_tx(port_no, b); else break;
    newline >>= 8;
  }
}


uint8_t uart_rx(uint8_t port_no) {
  register uint32_t da=(uint32_t)port_no*0x10000;
  while ( (*(volatile uint8_t*)(URT0_STA_b0+da) & 0x40) ==0); // waiting URT0_RXF==1
  return *(volatile uint8_t*)(URT0_RDAT_b0+da);
}

