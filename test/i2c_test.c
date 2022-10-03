#include "i2c_test.h"
#include "i2c.h"
#include "MG32x02z__RegAddress.h"
#include "MG32x02z_I2C.h"
#include "core.h"
#include "ulib.h"


// https://github.com/avishorp/TM1637

#define SEG_A   0b00000001
#define SEG_B   0b00000010
#define SEG_C   0b00000100
#define SEG_D   0b00001000
#define SEG_E   0b00010000
#define SEG_F   0b00100000
#define SEG_G   0b01000000
#define SEG_DP  0b10000000

#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80

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

static const uint8_t minusSegments = 0b01000000;


void i2c_test_master() {
  //register uint32_t ba=I2C0_Base + (I2C_PORT ? 0x10000 : 0); // base addr
  i2c_init(I2C_PORT);
  // Настройка выводов
  //RH(PB_CR10_h0) = (2 << 12) | (1 << 5) | 1; // PB10: I2C0_SCL, pull-up, open drain
  //RH(PB_CR11_h0) = (2 << 12) | (1 << 5) | 1; // PB11: I2C0_SDA, pull-up, open drain
  // Альтернативный вариант:
  RH(PC_CR8_h0) = (2 << 12) | (1 << 5) | 1; // PC8: I2C0_SCL, pull-up, open drain
  RH(PC_CR9_h0) = (2 << 12) | (1 << 5) | 1; // PC9: I2C0_SDA, pull-up, open drain
  // Настройка тактирования
  i2c_setup_clock(I2C_PORT,
      I2C_CLK_TMO_CKS_div64_h0 |  // CK_TMO: F(CK_PSC)/64 = 12500 Hz
      ((15 -1) << I2C_CLK_CK_PSC_shift_h0) | // CK_PSC: 12 MHz /15 = 800 kHz
      I2C_CLK_CK_DIV_div4_h0 |    // CK_I2Cx_INT: 200 kHz => F = 50 kHz
      I2C_CLK_CK_SEL_proc_h0      // I2Cx_CK_SEL: APB, 12 MHz
  );
  // Тайминг режима master
  //RW(I2C0_CR1_w) = 0x0202;
  // Настройка режима работы
  i2c_setup_mode(I2C_PORT,
      I2C_CR0_PDRV_SEL_1t_w |
      I2C_CR0_BUF_EN_enable_w |   // Режим работы через буфер
      I2C_CR0_MDS_i2c_w           // I2C : Single/Multi-Master/ Slave mode
  );

//  RH(I2C0_TMOUT_h0) =
//      (5 << I2C_TMOUT_TMO_CNT_shift_h0) |
//      I2C_TMOUT_TMO_MDS_scl_low_h0 |
//      I2C_TMOUT_TMO_EN_enable_h0;

  i2c_master_send(I2C_PORT,TM1637_I2C_COMM1);
  i2c_master_send(I2C_PORT,TM1637_I2C_COMM2);
  i2c_master_send(I2C_PORT,6); //i2c_master_send(I2C_PORT,7); i2c_master_send(I2C_PORT,7); i2c_master_send(I2C_PORT,7);
  i2c_master_send(I2C_PORT,TM1637_I2C_COMM3 + 0x6);
  //delay_ms(10);
  //i2c_master_send(I2C_PORT,0x55);
  //delay_ms(10);
  //i2c_master_send(I2C_PORT,0xAA,1);

  //while (! (RW(I2C0_STA_w) & I2C_STA_TXF_happened_w)) ;

}
