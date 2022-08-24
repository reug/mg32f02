// RAM based application framework for ARM Cortex-M0
// by reug@mail.ru, 2022

#include "MG32x02z__RegAddress.h"
#include "ulib.h"
#include "init.h"
#include "utils.h"
#include "adc_test.h"


// Mark first word with signature "Application is present" (nop; nop: 0x46c046c0)
__attribute__ ((section (".app_sign"))) __attribute__ ((__used__))
static uint32_t  app_sign=APP_SIGNATURE;


void uart_hdl() {
  char s[4]="< >";
  s[1]=uart_rx(PORT);
  uart_puts(PORT,s,UART_NEWLINE_CRLF);
}


/// Чтение значения CURRENT после сброса
void systick_test_read_current() {
  volatile register int32_t t0,t1;
  char s[12];
  *(volatile uint32_t*)CPU_SYST_CSR_w = 0; // Stop timer
  *(volatile uint32_t*)CPU_SYST_RVR_w = 15000; //(1 << 24) -1; // RELOAD
  *(volatile uint32_t*)CPU_SYST_CVR_w = 1; // Clear CURRENT
  //*(volatile uint32_t*)CPU_SYST_CSR_w = 1; // CLKSOURCE = 0 (0-CK_ST, 1-CPU), ENCNT=1
  t0 = *(volatile uint32_t*)CPU_SYST_CVR_w;
  strUint32hex(s,t0);
  uart_puts(PORT,s,UART_NEWLINE_CRLF);
  while (1);
}


void systick_test1() {
  volatile register int32_t t0,t1;
  char s[12];
  *(volatile uint32_t*)CPU_SYST_CSR_w = 0; // Stop timer
  *(volatile uint32_t*)CPU_SYST_RVR_w = 15000; //(1 << 24) -1; // RELOAD
  *(volatile uint32_t*)CPU_SYST_CVR_w = 1; // Clear CURRENT
  *(volatile uint32_t*)CPU_SYST_CSR_w = 1; // CLKSOURCE = 0 (0-CK_ST, 1-CPU), ENCNT=1
  while (1) {
    *(volatile uint32_t*)CPU_SYST_CVR_w = 1; // Clear CURRENT
    t0 = *(volatile uint32_t*)CPU_SYST_CVR_w;
    delay_ms(1);
    t1 = *(volatile uint32_t*)CPU_SYST_CVR_w;
    strUint32hex(s,t0-t1);
    uart_puts(PORT,s,UART_NEWLINE_CRLF);
    delay_ms(500);
  }
}


// Обработчик исключения SysTick
void systick_hdl() {
  *(volatile uint16_t*)PC_SC_h0 = 2; // set PC1
  *(volatile uint16_t*)PC_SC_h1 = 2; // clear PC1
}

// Функция тестирования SysTick Timer
void systick_test() {
  *(volatile uint16_t*)PC_CR1_h0 = 0x0002; // Выход таймера -> PC1
  SVC2(SVC_CHANDLER_SET,15,systick_hdl);   // Устанавливаем обработчик исключения 15
  *(volatile uint32_t*)CPU_SYST_CSR_w = 0; // Stop timer
  *(volatile uint32_t*)CPU_SYST_RVR_w = 1500-1; // RELOAD
  *(volatile uint32_t*)CPU_SYST_CVR_w = 1; // Clear CURRENT
  *(volatile uint32_t*)CPU_SYST_CSR_w = 3; // CLKSOURCE = 0 (External), TICKINT=1, ENCNT=1
  while (1) ;
}


// First function in application
__attribute__ ((section(".app"))) // put function in the begin of .text after signature word "app_sign"
__attribute__ ((noreturn))
void app() {
  // Настройка выводов для LED D1, D2:
  *(volatile uint16_t*)PB_CR13_h0 = 0x0002; // PB13 -> push-pull output
  *(volatile uint16_t*)PB_CR14_h0 = 0x0002; // PB14 -> push-pull output
  // Выключаем светодиоды:
  *(volatile uint16_t*)PB_SC_h1 = (1 << 13) | (1 << 14);

  setup_icko();
  //setup_ihrco();
  if (setup_xosc()) led1_flash(); else led2_flash();
  //while(1);

  // Настройка выводов URT0:
  //*(volatile uint16_t*)PB_CR8_h0 = (3 << 12) | 2; // PB8 -> URT0_TX, push pull output
  //*(volatile uint16_t*)PB_CR9_h0 = (3 << 12) | (1 << 5) | 3; // PB9 -> URT0_RX, pull-up resister enable, digital input
  // Альтернативный вариант:
  *(volatile uint16_t*)PB_CR2_h0 = (0xA << 12) | 2; // PB2 -> URT0_TX, push pull output
  *(volatile uint16_t*)PB_CR3_h0 = (0xA << 12) | (1 << 5) | 3; // PB3 -> URT0_RX, pull-up resister enable, digital input

  uart_init(PORT);
/*
  // Устанавливаем обработчик прерываний:
  SVC2(SVC_HANDLER_SET,20,uart_hdl);
  // Включаем прерывания в модуле URT0:
  *(volatile uint8_t*)URT0_INT_b0 = 0x40 | 0x01; // URT0_RX_IE | URT0_IEA
  // Включаем прерывание в модуле NVIC:
  *(volatile uint32_t*)CPU_ISER_w = (1 << 20); // SETENA 20
*/
  uart_puts(PORT,"Hello",UART_NEWLINE_CRLF);


  //adc_test_one();
  //adc_test_scan();
  //adc_test_sum();
  //adc_test_sum_cont();
  //adc_test_ivr24();
  //cmp_test();
  //cmp_test_ivref();
  //cmp_test_ivref_gen();
  systick_test();
  //systick_test2();
  //systick_test_read_current();

  while (1) {

  }
}
