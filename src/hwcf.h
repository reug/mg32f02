// Аппаратная конфигурация проекта

#include "MG32x02z__RegAddress.h"


//
// Первая схема на MG32F02A032AT20 (TSSOP20)
//
#ifdef HWCF_A032
#define HW_CLK_AHB        12000000    // MHz

#define HW_LED1_CRH0      PB_CR2_h0   // control register
#define HW_LED1_SCH0      PB_SC_h0    // set-clear register
#define HW_LED1_MASK      (1 << 2)    // bit mask

#define HW_LED2_CRH0      PB_CR3_h0   // control register
#define HW_LED2_SCH0      PB_SC_h0    // set-clear register
#define HW_LED2_MASK      (1 << 3)    // bit mask

// Настройка выводов URT0:
#define HW_URT0_SETTX     RH(PC_CR0_h0) = (0xA << 12) | 2
#define HW_URT0_SETRX     RH(PC_CR1_h0) = (0xA << 12) | (1 << 5) | 3

// Настройка выводов I2C0:
#define HW_I2C0_SETSCL    RH(PB_CR10_h0) = (2 << 12) | (1 << 5) | 1
#define HW_I2C0_SETSDA    RH(PB_CR11_h0) = (2 << 12) | (1 << 5) | 1

#endif // HWCF_A032

//
// Вторая схема на MG32F02A064AD48 (LQFP48)
//
#ifdef HWCF_A064
#define HW_CLK_AHB        12000000    // MHz

#define HW_LED1_CRH0      PB_CR13_h0  // control register
#define HW_LED1_SCH0      PB_SC_h0    // set-clear register
#define HW_LED1_MASK      (1 << 13)   // bit mask

#define HW_LED2_CRH0      PB_CR14_h0  // control register
#define HW_LED2_SCH0      PB_SC_h0    // set-clear register
#define HW_LED2_MASK      (1 << 14)   // bit mask

// Настройка выводов URT0:
//#define HW_URT0_SETTX RH(PB_CR8_h0) = (3 << 12) | 2
//#define HW_URT0_SETRX RH(PB_CR9_h0) = (3 << 12) | (1 << 5) | 3
// Альтернативный вариант:
#define HW_URT0_SETTX     RH(PB_CR2_h0) = (0xA << 12) | 2
#define HW_URT0_SETRX     RH(PB_CR3_h0) = (0xA << 12) | (1 << 5) | 3

// Настройка выводов I2C0:
//#define HW_I2C0_SETSCL    RH(PB_CR10_h0) = (2 << 12) | (1 << 5) | 1 // PB10: I2C0_SCL, pull-up, open drain
//#define HW_I2C0_SETSDA    RH(PB_CR11_h0) = (2 << 12) | (1 << 5) | 1 // PB11: I2C0_SDA, pull-up, open drain
// Альтернативный вариант:
#define HW_I2C0_SETSCL    RH(PC_CR8_h0) = (2 << 12) | (1 << 5) | 1 // PC8: I2C0_SCL, pull-up, open drain
#define HW_I2C0_SETSDA    RH(PC_CR9_h0) = (2 << 12) | (1 << 5) | 1 // PC9: I2C0_SDA, pull-up, open drain

#endif // HWCF_A064


//
// Auto setup. Do not edit
//
#define HW_LED1_SCH1      (HW_LED1_SCH0+2)
#define HW_LED2_SCH1      (HW_LED2_SCH0+2)
