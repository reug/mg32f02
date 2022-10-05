// TM1637 library for MG32F02 MCUs (ARM Cortex-M0)
// by reug@mail.ru, 2022

#include "tm1637.h"
#include "core.h"
#include "ulib.h"
#include "MG32x02z__RegAddress.h"

// References:
// 1. https://electro-dan.co.uk/electronics/PIC_simple_thermometer.aspx
// 2. https://github.com/avishorp/TM1637

#define SEG_A   0b00000001
#define SEG_B   0b00000010
#define SEG_C   0b00000100
#define SEG_D   0b00001000
#define SEG_E   0b00010000
#define SEG_F   0b00100000
#define SEG_G   0b01000000
#define SEG_DP  0b10000000

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const uint8_t digitToSegment[] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
};

static const uint8_t minusSegment = 0b01000000;




void stwi_bitdelay() {
  uint32_t k=30;
  while (--k) __NOP();
}

//void stwi_edgedelay() {
//  uint32_t k=10;
//  while (--k) __NOP();
//}

void stwi_scl(uint8_t s) {
  RH(s ? PA_SC_h0 : PA_SC_h1) = (1 << 8); // PA8
  //RH(s ? PC_SC_h0 : PC_SC_h1) = (1 << 8); // PC8
}

void stwi_sda(uint8_t s) {
  RH(s ? PA_SC_h0 : PA_SC_h1) = (1 << 9); // PA9
  //RH(s ? PC_SC_h0 : PC_SC_h1) = (1 << 9); // PC9
}

void stwi_scl_dir(uint8_t d) {
  RH(PA_CR8_h0) = d; // PA8: I2C0_SCL
  //RH(PC_CR8_h0) = d; // PC8: I2C0_SCL
}

void stwi_sda_dir(uint8_t d) {
  RH(PA_CR9_h0) = d; // PA9: I2C0_SDA
  //RH(PC_CR9_h0) = d; // PC9: I2C0_SDA
}

uint8_t stwi_sda_read() {
  return (RH(PA_IN_h0) & (1 << 9)) ? 1 : 0; // PA9
  //return (RH(PC_IN_h0) & (1 << 9)) ? 1 : 0; // PC9
}

void stwi_start() {
  //stwi_scl_dir(INPUT); stwi_sda_dir(INPUT);
  //stwi_edgedelay();
  stwi_sda_dir(OUTPUT); // SDA low
  stwi_bitdelay();
}

void stwi_stop() {
  //stwi_scl_dir(OUTPUT); // SDA low
	//stwi_bitdelay();
	stwi_sda_dir(OUTPUT); // SDA low
	stwi_bitdelay();
	stwi_scl_dir(INPUT); // SCL high
	stwi_bitdelay();
	stwi_sda_dir(INPUT); // SDA high
	stwi_bitdelay();
}


uint8_t stwi_send(uint8_t b) {
  uint8_t data = b;
  uint8_t i;

  // 8 Data Bits
  for (i= 0; i < 8; i++) {
    stwi_scl_dir(OUTPUT); // CLK low
    stwi_bitdelay();

	  // Set data bit: // LSB first
    stwi_sda_dir((data & 0x01) ? INPUT : OUTPUT);
    stwi_bitdelay();

    stwi_scl_dir(INPUT); // CLK high
    stwi_bitdelay();
    data >>= 1;
  }

  // Wait for acknowledge
  stwi_scl_dir(OUTPUT); // CLK low
  stwi_sda_dir(INPUT);
  stwi_bitdelay();

  stwi_scl_dir(INPUT); // CLK high
  stwi_bitdelay();
  i = stwi_sda_read();
  if (i == 0) stwi_sda_dir(OUTPUT);

  stwi_bitdelay();
  stwi_scl_dir(OUTPUT); // CLK low
  stwi_bitdelay();

  return i;
}


void tm1637_init() {
  // Init lines
  stwi_scl(0);  stwi_sda(0);
  //stwi_scl_dir(OUTPUT); stwi_sda_dir(OUTPUT); stwi_bitdelay();
}


uint8_t tm1637_put_dec(uint16_t v) {
  uint8_t a=0,d,i=4;
  uint32_t c;

  stwi_start();
  if (stwi_send(TM1637_CMD_WDAA)) goto failure;
  stwi_stop();

  c=bcd16(v);
  stwi_start();
  stwi_send(TM1637_CMD_ADDR);
  do {
    d = (c >> 12) & 0xF; // get dec digit
    if (a==0) a = d ? digitToSegment[d] : 0;
    else a=digitToSegment[d];
    if (stwi_send(a)) goto failure;
    c <<= 4;
  }
  while (--i);
  stwi_stop();
  return 0;

failure:
  stwi_stop();
  return 1;
}


uint8_t tm1637_brightness(uint8_t b) {
  uint8_t r;
  stwi_start();
  r=stwi_send(TM1637_CMD_CTRL | b);
  stwi_stop();
  return r;
}


void tm1637_test() {
  uint16_t d=0;
  tm1637_init();
  tm1637_brightness(15);
  while (1) {
    for (; d<2000; d++) {
      if (tm1637_put_dec(d)) return;
      delay_ms(100);
    }
    for (; d!=0; d--) {
      if (tm1637_put_dec(d)) return;
      delay_ms(100);
    }
  }
}
