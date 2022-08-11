// RAM based application framework for ARM Cortex-M0
// by reug@mail.ru, 2022

#include "MG32x02z__RegAddress.h"
#include "ulib.h"
#include "api.h"
#include "uart.h"
#include "adc.h"

#define PORT 0


// Mark first word with signature "Application is present" (nop; nop: 0x46c046c0)
__attribute__ ((section (".app_sign"))) __attribute__ ((__used__))
static uint32_t  app_sign=APP_SIGNATURE;


void uart_hdl() {
  char s[4]="< >";
  s[1]=uart_rx(PORT);
  uart_puts(PORT,s,UART_NEWLINE_CRLF);
}


// CK_ICKO output through PC0 pin
void setup_icko() {
  *(volatile uint16_t*)PC_CR0_h0 = (0x1 << 12) | 2; // PC0 -> ICKO, push pull output
  *(volatile uint16_t*)CSC_KEY_h0 = 0xA217; // unlock access to CSC regs
  //*(volatile uint32_t*)CSC_CKO_w = (0x0 << 4) | 1; // CK_MAIN, DIV=1, CSC_CKO_EN = 1
  //*(volatile uint32_t*)CSC_CKO_w = (0x3 << 4) | 1; // CK_HS, DIV=1, CSC_CKO_EN = 1
  *(volatile uint32_t*)CSC_CKO_w = (0x0 << 4) | (3 << 2) | 1; // CK_MAIN, DIV=8, CSC_CKO_EN = 1
  *(volatile uint16_t*)CSC_KEY_h0 = 0x1111; // lock access to CSC regs
}


// Установка частоты IHRCO на 11,0592 МГц
void setup_ihrco() {
  *(volatile uint16_t*)CSC_KEY_h0 = 0xA217; // unlock access to CSC regs
  *(volatile uint32_t*)CSC_CR0_w |= (1 << 18); // CSC_IHRCO_SEL = 1 (11.0592 MHz)
  *(volatile uint16_t*)CSC_KEY_h0 = 0x1111; // lock access to CSC regs
}


// Включение XOSC генератора
void setup_xosc() {
  uint32_t d;
  // Setup pins XIN (PC13) & XOUT (PC14):
  *(volatile uint16_t*)PC_CR13_h0 = (1 << 12); // PC13 -> XIN
  *(volatile uint16_t*)PC_CR14_h0 = (1 << 12); // PC14 -> XOUT
  while (! (*(volatile uint8_t*)CSC_STA_b0 & 2)); // waiting CSC_XOSCF == 1 (XOSC ready)
  *(volatile uint16_t*)CSC_KEY_h0 = 0xA217; // unlock access to CSC regs
  d=*(volatile uint32_t*)CSC_CR0_w;
  d &= ~(3 << 10); // clear bits 10,11
  d |= (1 << 10); // set CSC_HS_SEL = 0b01 (XOSC)
  *(volatile uint32_t*)CSC_CR0_w = d;
  *(volatile uint16_t*)CSC_KEY_h0 = 0x1111; // lock access to CSC regs
}


// Включение умножения частоты на основе PLL (x2)
void setup_pll() {
  uint16_t d;
  *(volatile uint16_t*)CSC_KEY_h0 = 0xA217; // unlock access to CSC regs

  // CSC_PLLI_DIV = 2, CK_PLLI = 6 MHz (CK_HS/2)
  // CSC_PLLO_DIV = 2 , CK_PLLO = 48 MHz (CK_PLL/2)
  //*(volatile uint8_t*)CSC_DIV_b0 = 0b00100001; // CK_PLLO = 48 MHz (CK_PLL/2)
  *(volatile uint8_t*)CSC_DIV_b0 = 0b00000001; // CK_PLLO = 24 MHz (CK_PLL/2)

  // CSC_PLL.CSC_PLL_MUL = 0, PLL_MULL = 16 (DEFAULT), CK_PLL = 96 MHz (CK_PLII*16)
  //*(volatile uint8_t*)CSC_PLL_b1 |= 1; // CSC_PLL_MUL = 1, PLL_MULL = 24, CK_PLL = 144 MHz (CK_PLII*24)

  *(volatile uint8_t*)CSC_CR0_b0 |= (1 << 5); // CSC_PLL_EN = 1

  while (! (*(volatile uint8_t*)CSC_STA_b0 & (1 << 6))); // waiting CSC_PLLF == 1 (PLL ready)

  d=*(volatile uint16_t*)CSC_CR0_h0;
  d &= ~(3 << 14); // clear bits 14,15
  d |= (2 << 14); // CSC_MAIN_SEL = 2 (CK_PLLO)
  *(volatile uint16_t*)CSC_CR0_h0 = d;

  *(volatile uint16_t*)CSC_KEY_h0 = 0x1111; // lock access to CSC regs
}


// First function in application
__attribute__ ((section(".app"))) // put function in the begin of .text after signature word "app_sign"
__attribute__ ((noreturn))
/*
// Вариант без прерывания
void app() {
  char s[4]="< >";

  uart_init(PORT);
  uart_puts(PORT,"Hello",UART_NEWLINE_CRLF);
  while (1) {
    s[1]=uart_rx(PORT);
    uart_puts(PORT,s,UART_NEWLINE_CRLF);
  }

}
*/

// Вариант с прерыванием и портом 0
void app() {
  //uint16_t i;
  char s[8];

  //*((volatile uint16_t*)(PB1_CR_ADR))=0x0002;
  *(volatile uint16_t*)PB_CR2_h0 = 0x0002; // PB2 -> push-pull output

  uart_init(PORT);
  SVC2(SVC_HANDLER_SET,20,uart_hdl);

  // включаем прерывания в модуле URT0
  *(volatile uint8_t*)URT0_INT_b0 = 0x40 | 0x01; // URT0_RX_IE | URT0_IEA
  // включаем прерывание в модуле NVIC
  *(volatile uint32_t*)CPU_ISER_w = (1 << 20); // SETENA 20

  uart_puts(PORT,"Hello",UART_NEWLINE_CRLF);
  adc_init();
  //i=49083;
  //i=bcd16(i);
  //strUint16(s,5,i);
  //uart_puts(PORT, s, UART_NEWLINE_CRLF);


  while (1) {
    adc_start_one(8);
    *(volatile uint16_t*)PB_SC_h0 = 0x0004; // set bit 2
    delay_ms(50);

    strUint16hex(s,bcd16(adc_samp()));
    uart_puts(PORT, s, UART_NEWLINE_CRLF);
    *(volatile uint16_t*)PB_SC_h1 = 0x0004; // clear bit 2
    delay_ms(50);
  }

}

/*
// Вариант с прерыванием и портом 1
void app() {
  setup_icko();
  //setup_ihrco();
  setup_xosc();
  //setup_pll();

  //*((volatile uint16_t*)(PB1_CR_ADR))=0x0002;
  *(volatile uint16_t*)PB_CR2_h0 = 0x0002; // PB2 -> push-pull output

  uart_init(PORT);
  SVC2(SVC_HANDLER_SET,21,uart_hdl);

  // включаем прерывания в модуле URT1
  *(volatile uint8_t*)URT1_INT_b0 = 0x40 | 0x01; // URT1_RX_IE | URT1_IEA
  // включаем прерывание в модуле NVIC
  *(volatile uint32_t*)CPU_ISER_w = (1 << 21); // SETENA 21
  uart_puts(PORT,"Hello",UART_NEWLINE_CRLF);

  while (1) {
    *(volatile uint16_t*)PB_SC_h0 = 0x0004; // set bit 2
    delay_ms(250);
    *(volatile uint16_t*)PB_SC_h1 = 0x0004; // clear bit 2
    delay_ms(250);
  }

}
*/
