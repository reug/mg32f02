// RAM based application framework for ARM Cortex-M0
// by reug@mail.ru, 2022

#include "core.h"
#include "hwcf.h"
#include "ulib.h"
#include "utils.h"
#include "init.h"
#include "i2c.h"
#include "ic/ds3231.h"
#include "ic/tm1637.h"


// Mark first word with signature "Application is present" (nop; nop: 0x46c046c0)
__attribute__ ((section (".app_sign"))) __attribute__ ((__used__))
static uint32_t  app_sign=APP_SIGNATURE;


void uart_hdl() {
  char s[4]="< >";
  s[1]=uart_rx(PORT);
  uart_puts(PORT,s,UART_NEWLINE_CRLF);
}


void clock() {
  uint32_t d;

  //register uint32_t ba=I2C0_Base + (DS3231_PORT ? 0x10000 : 0); // base addr
  i2c_init(DS3231_PORT);
  // Настройка выводов I2C:
  HW_I2C0_SETSCL;
  HW_I2C0_SETSDA;

  // Настройка тактирования
  i2c_setup_clock(DS3231_PORT,
      I2C_CLK_TMO_CKS_div64_h0 |  // CK_TMO: F(CK_PSC)/64 = 12500 Hz
      ((15 -1) << I2C_CLK_CK_PSC_shift_h0) | // CK_PSC: 12 MHz /15 = 800 kHz
      I2C_CLK_CK_DIV_div8_h0 |    // CK_I2Cx_INT: 200 kHz => F = 50 kHz
      I2C_CLK_CK_SEL_proc_h0      // I2Cx_CK_SEL: APB, 12 MHz
  );
  // Тайминг режима master
  //RW(I2C0_CR1_w) = 0x0202;
  // Настройка режима работы
  i2c_setup_mode(DS3231_PORT,
      I2C_CR0_PDRV_SEL_1t_w |
      I2C_CR0_BUF_EN_enable_w |   // Режим работы через буфер
      I2C_CR0_MDS_i2c_w           // I2C : Single/Multi-Master/ Slave mode
  );

  //clock_set_bcd(0x181445);
  tm1637_init(); tm1637_brightness(10);

  while (1) {
    d=clock_get_bcd();
    if (i2c_get_tmout(DS3231_PORT)) {
      d=i2c_get_status(DS3231_PORT);
      debug32hex('S',d); i2c_print_status(d);
      led2_flash();
      i2c_clr_status(DS3231_PORT, I2C_STA_TMOUTF_mask_w);
      d=0xE001;
    }
    else {
      debug32hex('T',d);
      d >>= 8; // выводим часы:минуты
    }
    tm1637_put_hex(d & 0xFFFF,1);
    delay_ms(500);
    tm1637_put_hex(d & 0xFFFF,0);
    delay_ms(500);
  }

}


// First function in application
__attribute__ ((section(".app"))) // put function in the begin of .text after signature word "app_sign"
__attribute__ ((noreturn))
void app() {

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
  uart_puts(PORT,"Hello",UART_NEWLINE_CRLF);
  clock();

  while (1) led_blink();
}
