// RAM based application framework for ARM Cortex-M0
// by reug@mail.ru, 2022

#include "core.h"
#include "ulib.h"
#include "utils.h"
//#include "adc_test.h"
//#include "test/st_rtc_test.h"
//#include "test/wdt_test.h"
#include "test/timer_test.h"


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
void app() {

/*
  // Проверка выводов PB8 и PB9
  RH(PB_CR8_h0 = 2; // PB8 -> push pull output
  RH(PB_CR9_h0 = 2; // PB9 -> push pull output
  RH(PB_SC_h0 = (3 << 8);
  while (1);
*/

  // Настройка выводов для LED D1, D2:
  RH(PB_CR13_h0) = 0x0002; // PB13 -> push-pull output
  RH(PB_CR14_h0) = 0x0002; // PB14 -> push-pull output
  // Выключаем светодиоды:
  RH(PB_SC_h1) = (1 << 13) | (1 << 14);

  setup_icko();
  //setup_ihrco();
  if (setup_xosc()) led1_flash(); else led2_flash();

  // Настройка выводов URT0:
  //RH(PB_CR8_h0) = (3 << 12) | 2; // PB8 -> URT0_TX, push pull output
  //RH(PB_CR9_h0) = (3 << 12) | (1 << 5) | 3; // PB9 -> URT0_RX, pull-up resister enable, digital input
  // Альтернативный вариант:
  RH(PB_CR2_h0) = (0xA << 12) | 2; // PB2 -> URT0_TX, push pull output
  RH(PB_CR3_h0) = (0xA << 12) | (1 << 5) | 3; // PB3 -> URT0_RX, pull-up resister enable, digital input

  uart_init(PORT);
/*
  // Устанавливаем обработчик прерываний URT0:
  SVC2(SVC_HANDLER_SET,20,uart_hdl);
  // Включаем прерывания в модуле URT0:
  RB(URT0_INT_b0) = 0x40 | 0x01; // URT0_RX_IE | URT0_IEA
  // Включаем прерывание в модуле NVIC:
  RW(CPU_ISER_w) = (1 << 20); // SETENA 20
*/
  uart_puts(PORT,"Hello",UART_NEWLINE_CRLF);
  tm00_test();

  while (1);
}
