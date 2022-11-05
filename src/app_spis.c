// RAM based application framework for ARM Cortex-M0
// by reug@mail.ru, 2022

#include "core.h"
#include "hwcf.h"
#include "ulib.h"
#include "utils.h"
#include "init.h"
#include "MG32x02z_SPI.h"
#include "spi.h"


// Mark first word with signature "Application is present" (nop; nop: 0x46c046c0)
__attribute__ ((section (".app_sign"))) __attribute__ ((__used__))
static uint32_t  app_sign=APP_SIGNATURE;


void uart_hdl() {
  char s[4]="< >";
  s[1]=uart_rx(PORT);
  uart_puts(PORT,s,UART_NEWLINE_CRLF);
}


inline
void mark_on() {RH(PB_SC_h0) = (1 << 8);}

inline
void mark_off() {RH(PB_SC_h1) = (1 << 8);}

#define TXBUF_SIZE 16
char s[TXBUF_SIZE]="ABCDEFGHIJKLMNO";    // Буфер для данных

/// Обработчик прерывания INT_SPI0
void spi_hdl() {
  uint8_t i;
  uint8_t n; // Число запрашиваемых байт

  mark_on();
  n=RB(SPI0_RDAT_b0);
  spi_flush_rx();
  if (n<TXBUF_SIZE) {
    spi_flush_tx();
    for (i=0; i<n; i++) spi_slave_tx(s[i]);
  }
  mark_off();
  if (RW(SPI0_STA_b1) & SPI_STA_ROVRF_mask_b1) {
    mark_on(); mark_off();
    //spi_flush_rx(); // сбрасываем буфер приема
    RW(SPI0_STA_w) = SPI_STA_ROVRF_mask_w; // сбрасываем флаг
  }

}


void spi_test_slave() {
  // Настройка выводов:
  HW_SPI0_SETMISO;  HW_SPI0_SETMOSI;  HW_SPI0_SETSCK;  HW_SPI0_SETNSS;
  // Инициализация, настройка тактирования:
  spi_init();
  // Настройка режима работы
  spi_setup_mode(
    //SPI_NSS_PEN |
    //SPI_NSSO_INV | SPI_NSS_SWEN | // software NSS control
    SPI_NSSI_EN | SPI_SLAVE | SPI_MSB | SPI_CPHA_LE | SPI_CPOL_LOW
    //| SPI_CR0_DOUT_MDS_enable_w // надо включить, если нет резистора подтяжки
  );
  RW(SPI0_CR2_w) =
    SPI_CR2_TXUPD_EN_enable_w |     // включаем прямое обновление буфера TX, чтобы успевать обновлять данные
    (8 << SPI_CR2_DSIZE_shift_w) |  // размер кадра в битах
    SPI_CR2_RX_TH_1_byte_w;         // порог приема
  // Установка обработчика прерывания:
  SVC2(SVC_HANDLER_SET,24,spi_hdl);
  // Разрешение прерывания, в том числе, в NVIC:
  spi_setup_int(SPI_INT_RX_IE_enable_h0);
  //spi_setup_int(SPI_INT_ROVR_IE_enable_h0 | SPI_INT_RX_IE_enable_h0);
  while (1) ;
}


// First function in application
__attribute__ ((section(".app"))) // put function in the begin of .text after signature word "app_sign"
__attribute__ ((noreturn))
void app() {

  setup_icko();
  setup_xosc();

  mark_off();
  RH(PB_CR8_h0) = 0x0002; // pin -> push-pull output

  gpl_init(); // Включаем модуль GPL для целочисленного аппаратного деления и CRC

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
  spi_test_slave();


  //while (1) led_blink();
  while (1);
}
