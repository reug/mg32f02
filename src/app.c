// RAM based application framework for ARM Cortex-M0
// by reug@mail.ru, 2022

#include "MG32x02z__RegAddress.h"
#include "ulib.h"
#include "api.h"
#include "uart.h"
#include "adc.h"
#include "init.h"

#define PORT 0


// Mark first word with signature "Application is present" (nop; nop: 0x46c046c0)
__attribute__ ((section (".app_sign"))) __attribute__ ((__used__))
static uint32_t  app_sign=APP_SIGNATURE;


void uart_hdl() {
  char s[4]="< >";
  s[1]=uart_rx(PORT);
  uart_puts(PORT,s,UART_NEWLINE_CRLF);
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
  uint16_t i;
  char s[8];

  setup_icko();
  //setup_ihrco();
  if (!setup_xosc()) {
    *(volatile uint16_t*)PB_CR14_h0 = 0x0002; // PB14 -> push-pull output
    *(volatile uint16_t*)PB_SC_h0 = (1 << 14);  // red light
  }

  // Pins: LED D1
  *(volatile uint16_t*)PB_CR13_h0 = 0x0002; // PB13 -> push-pull output

  // Pins: URT0
  *(volatile uint16_t*)PB_CR8_h0 = (3 << 12) | 2; // PB8 -> URT0_TX, push pull output
  *(volatile uint16_t*)PB_CR9_h0 = (3 << 12) | (1 << 5) | 3; // PB9 -> URT0_RX, pull-up resister enable, digital input

  uart_init(PORT);
  SVC2(SVC_HANDLER_SET,20,uart_hdl);

  // включаем прерывания в модуле URT0
  *(volatile uint8_t*)URT0_INT_b0 = 0x40 | 0x01; // URT0_RX_IE | URT0_IEA
  // включаем прерывание в модуле NVIC
  *(volatile uint32_t*)CPU_ISER_w = (1 << 20); // SETENA 20

  uart_puts(PORT,"Hello",UART_NEWLINE_CRLF);

  adc_vbuf();
  adc_init();
  //adc_ivr24();

  //i=0;
  while (1) {
    //adc_start_one(8);
    adc_start_one_int(3);
    *(volatile uint16_t*)PB_SC_h0 = (1 << 13); // set bit 2
    delay_ms(100);

//    strUint16(s,2,i);
//    uart_puts(PORT, s, UART_NEWLINE_CRLF);

    strUint16(s,5,adc_samp());
    uart_puts(PORT, s, UART_NEWLINE_CRLF);

    *(volatile uint16_t*)PB_SC_h1 = (1 << 13); // clear bit 2
    delay_ms(100);

    //if (++i>12) i=0;
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
