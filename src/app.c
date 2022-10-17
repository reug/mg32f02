// RAM based application framework for ARM Cortex-M0
// by reug@mail.ru, 2022

#include "core.h"
#include "hwcf.h"
#include "ulib.h"
#include "utils.h"
#include "init.h"
//#include "adc_test.h"
//#include "test/st_rtc_test.h"
//#include "test/wdt_test.h"
//#include "test/timer_test.h"
//#include "nco.h"
#include "test/i2c_test.h"


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

//  // Проверка вывода PB10 (15)
//  RH(PB_CR10_h0) = 0x0002; // PB10 -> push-pull output
//  RH(PB_CR11_h0) = 0x0002; // PB11 -> push-pull output
//  // Выключаем светодиоды:
//  RH(PB_SC_h1) = (1 << 10) | (1 << 11);
//
//  while (1) {
//    delay_ms(1000);
//    RH(PB_SC_h0) = (1 << 10);
//    delay_ms(1000);
//    RH(PB_SC_h1) = (1 << 10);
//  }
  // Настройка выводов для LED D1, D2:
  RH(HW_LED1_CRH0) = 0x0002; // pin -> push-pull output
  RH(HW_LED2_CRH0) = 0x0002; // pin -> push-pull output
  // Выключаем светодиоды:
  RH(HW_LED1_SCH1) = HW_LED1_MASK;
  RH(HW_LED2_SCH1) = HW_LED2_MASK;

  setup_icko();
  //setup_ihrco();
  if (setup_xosc()) led1_flash(); else led2_flash();

  gpl_init(); // Включаем модуль GPL для целочисленного аппаратного деления

  // Настройка выводов URT0:
  //RH(PB_CR8_h0) = (3 << 12) | 2; // PB8 -> URT0_TX, push pull output
  //RH(PB_CR9_h0) = (3 << 12) | (1 << 5) | 3; // PB9 -> URT0_RX, pull-up resister enable, digital input
  // Альтернативный вариант:
  //RH(PB_CR2_h0) = (0xA << 12) | 2; // PB2 -> URT0_TX, push pull output
  //RH(PB_CR3_h0) = (0xA << 12) | (1 << 5) | 3; // PB3 -> URT0_RX, pull-up resister enable, digital input
  HW_URT0_SETTX;
  HW_URT0_SETRX;

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
  //tm00_test();
  //tm00_test_lowfreq_separate();
  //tm00_test_lowfreq_fullcnt();
  //timer_test_tm1x();
  //timer_test_tm26();
  //timer_test_gen(1500,1000,570); // Такт 1 мс (1000 Гц), T=1 с, P=0.1 c

  //timer_test_freq();
  //timer_test_capture();
  //timer_test_pwm();
  i2c_test_master_setup();
  //i2c_test_master_w1r_ds3231();
  //i2c_test_master_w1r();
  i2c_test_master_wN(4);
  //tm1637_test();

/*
  // NCO test
  RH(PB_CR3_h0) = (0x3 << 12) | 2; // PB3 -> NCO_P0 output
  nco_init(NCO_MODE_FDC);
  //nco_set(1024);    // 117187 Hz
  //nco_set(8739);  // 100000 Hz
  nco_set(2^10); // 115200 Hz
*/

  while (1) led_blink();
}
