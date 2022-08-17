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

// application global flags
uint32_t gf;

// adc results
uint16_t ad[16];


void debug(char label, uint16_t d) {
  char s[32];
  s[0]=label; s[1]=' ';
  strUint16hex(s+2,d); s[6]=' ';
  strUint16(s+7,5,d);
  uart_puts(PORT, s, UART_NEWLINE_CRLF);
}


void led1_flash() {
  *(volatile uint16_t*)PB_SC_h0 = (1 << 13); // set bit 14
  delay_ms(100);
  *(volatile uint16_t*)PB_SC_h1 = (1 << 13); // clear bit 14
  delay_ms(100);
}

void led2_flash() {
  *(volatile uint16_t*)PB_SC_h0 = (1 << 14); // set bit 14
  delay_ms(100);
  *(volatile uint16_t*)PB_SC_h1 = (1 << 14); // clear bit 14
  delay_ms(100);
}


void uart_hdl() {
  char s[4]="< >";
  s[1]=uart_rx(PORT);
  uart_puts(PORT,s,UART_NEWLINE_CRLF);
}


void adc_hdl() {
  *(volatile uint8_t*)ADC0_STA_b0 = 0x08;  // clear ADC0_E1CNVF flag
  gf++;
  return;
  uint32_t c,d;
  //if (*(volatile uint8_t*)ADC0_STA_b0 & 0x08) { // ADC0_E1CNVF==1

    d = *(volatile uint32_t*)ADC0_DAT0_w;
    c = d >> 28;

    //*(volatile uint16_t*)ADC0_STA_h0 = 0xFFFF;  // clear all flags
    *(volatile uint8_t*)ADC0_STA_b0 = 0x08;  // clear ADC0_E1CNVF flag

    //debug('C',c);
    //debug('D',d);
    gf |= (1 << c);
    ad[c]=d;
  //}
//  if (*(volatile uint8_t*)ADC0_STA_b0 & (1 << 5)) { // ADC0_ESCNVF==1
//
//    //led2_flash();
//  }
}


// First function in application
__attribute__ ((section(".app"))) // put function in the begin of .text after signature word "app_sign"
__attribute__ ((noreturn))
void app() {
  uint32_t d,i;
  char s[16];

  gf=0;
  for (i=0; i<16; i++) ad[i]=0;

  // Pins: LED D1 , D2
  *(volatile uint16_t*)PB_CR13_h0 = 0x0002; // PB13 -> push-pull output
  *(volatile uint16_t*)PB_CR14_h0 = 0x0002; // PB14 -> push-pull output
  // Switch off leds
  *(volatile uint16_t*)PB_SC_h1 = (1 << 13) | (1 << 14);

  setup_icko();
  //setup_ihrco();
  //if (setup_xosc()) led1_flash(); else led2_flash();

  // Pins: URT0
  //*(volatile uint16_t*)PB_CR8_h0 = (3 << 12) | 2; // PB8 -> URT0_TX, push pull output
  //*(volatile uint16_t*)PB_CR9_h0 = (3 << 12) | (1 << 5) | 3; // PB9 -> URT0_RX, pull-up resister enable, digital input
  // Альтернативный вариант:
  *(volatile uint16_t*)PB_CR2_h0 = (0xA << 12) | 2; // PB8 -> URT0_TX, push pull output
  *(volatile uint16_t*)PB_CR3_h0 = (0xA << 12) | (1 << 5) | 3; // PB9 -> URT0_RX, pull-up resister enable, digital input

  //*(volatile uint16_t*)PB_CR10_h0 = (3 << 12) | (1 << 5) | 3; // PB9 -> URT0_RX, pull-up resister enable, digital input

  uart_init(PORT);
  SVC2(SVC_HANDLER_SET,20,uart_hdl);
  // включаем прерывания в модуле URT0
  *(volatile uint8_t*)URT0_INT_b0 = 0x40 | 0x01; // URT0_RX_IE | URT0_IEA
  // включаем прерывание в модуле NVIC
  *(volatile uint32_t*)CPU_ISER_w = (1 << 20); // SETENA 20

  uart_puts(PORT,"Hello",UART_NEWLINE_CRLF);
  delay_ms(10);


  adc_init();
  SVC2(SVC_HANDLER_SET,10,adc_hdl);
  // включаем прерывания в модуле ADC
  *(volatile uint16_t*)ADC0_INT_h0 = (1 << 3) | 1; // ADC0_E1CNV_IE | ADC0_IEA
  // включаем прерывание в модуле NVIC
  *(volatile uint32_t*)CPU_ISER_w = (1 << 10); // SETENA 10

  //adc_vbuf();
  //adc_ivr24();
  // Увеличиваем время сэмплирования:
  //*(volatile uint8_t*)ADC0_CR0_b2 = 50; // ADC0_SMP_SEL


//  // Включение термометра
//  *(volatile uint8_t*)ADC0_ANA_b1 |= 0x80; // ADC0_TS_AUTO = 1
//  *(volatile uint8_t*)ADC0_ANA_b0 |= 0x08; // ADC0_TS_EN = 1


  //while (1) {

    *(volatile uint16_t*)PB_SC_h0 = (1 << 13); // set bit 2
    delay_ms(50);
    *(volatile uint16_t*)PB_SC_h1 = (1 << 13); // clear bit 2

    adc_start_mask(0x0f00);
    //adc_start_mask((1 << 9));
    //adc_start_mask((1 << 8) | (1 << 9));
    delay_ms(100);

    //adc_start_one_int(3);
    //adc_start_one(8);
    //d = adc_samp();
    //d = adc_measure_sum(9 | 0x10);
    //d = adc_measure_sum_cont(8);

//    strUint16hex(s,d); s[4]=' '; strUint16(s+5,5,d);
//    uart_puts(PORT, s, UART_NEWLINE_CRLF);

    uart_puts(PORT,"------------",UART_NEWLINE_CRLF);
    debug('C',gf);
    debug('8',ad[8]);
    debug('9',ad[9]);

    delay_ms(1000);
  //}

  while (1);

}
