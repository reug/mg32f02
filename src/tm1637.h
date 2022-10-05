// TM1637 library for MG32F02 MCUs (ARM Cortex-M0)
// by reug@mail.ru, 2022

#ifndef TM1637_H
#define TM1637_H

#include <stdint.h>

// TM1637
// Максимальная частота сигнала SCL - 250 кГц.
// Первый отправленный байт после состояния START рассматривается как команда.
// При возникновении состояния STOP последовательный интерфейс TM1637 сбрасывается в исходное состояние.

// TM1637 Commands:
#define TM1637_CMD_WDAA   0x40 ///< Write data to display register (Automatic address adding)
#define TM1637_CMD_WDFA   0x44 ///< Write data to display register (Fixed address)
#define TM1637_CMD_CTRL   0x80 ///< Display and control command setting
#define TM1637_CMD_ADDR   0xC0 ///< Address command setting

// ******** Low-level Software two-wire interface (STWI) ********

/// Line direction
enum STWI_Direction {
  UNSET = 0,
  OUTPUT = 1,
  //INPUT = 3 // input
  INPUT = (3 | (1<<5)) // input + pull-up
};

/// Bit delay (internal)
void stwi_bitdelay();

/// Sets SCL line (internal)
void stwi_scl(uint8_t s);

/// Sets SDA line (internal)
void stwi_sda(uint8_t s);

/// Sets SCL direction (internal)
void stwi_scl_dir(uint8_t d);

/// Sets SDA direction (internal)
void stwi_sda_dir(uint8_t d);

/// Reads SDA line (internal)
uint8_t stwi_sda_read();

/// Does Start
void stwi_start();

/// Does Stop
void stwi_stop();

/// Sends byte (LE format, LSB first).
/// Returns 0 if ACK, 1 - if NAK.
uint8_t stwi_send(uint8_t d);


// ******** TM1637 API ********

/// Init TM1637
void tm1637_init();

/// Prints 4-digit interger
/// Returns 0 if ACK, 1 - if NAK.
uint8_t tm1637_put_dec(uint16_t v);

/// Sets brightness
/// Returns 0 if ACK, 1 - if NAK.
uint8_t tm1637_brightness(uint8_t b);

void tm1637_test();


#endif //  TM1637_H
