#include "rtc.h"
#include "MG32x02z__RegAddress.h"
#include "MG32x02z_CSC.h"
#include "MG32x02z_RTC.h"
#include "MG32x02z_SYS.h"
#include "MG32x02z_CPU.h"
#include "core.h"


void rtc_init() {
  RH(CSC_KEY_h0) = 0xA217; // unlock access to CSC regs
  RB(CSC_APB0_b0) |= CSC_APB0_RTC_EN_enable_b0; // CSC_RTC_EN = 1
  RH(CSC_KEY_h0) = 0; // lock access to CSC regs
}


// NON-CMSIS
void rtc_set_int(uint8_t flags) {
  RH(RTC_KEY_h0) = 0xA217; // unlock access to regs
  // включаем прерывания в модуле:
  RB(RTC_INT_b0) = flags | RTC_INT_IEA_enable_b0; // RTC_IEA
  RH(RTC_KEY_h0) = 0; // lock access to regs
  // включаем прерывание INT_SYS:
  RB(SYS_INT_b0) = 1; // SYS_IEA=1
  // включаем прерывание в модуле NVIC:
  RW(CPU_ISER_w) = (1 << 1); // SETENA 1
}

/*
// CMSIS
void rtc_set_int(uint8_t flags) {
  RTC->KEY.MBIT.KEY = 0xA217; // unlock access to regs
  // включаем прерывания в модуле:
  RTC->INT.B[0] = flags | RTC_INT_IEA_enable_b0; // RTC_IEA
  RTC->KEY.MBIT.KEY = 0; // lock access to regs
  // включаем прерывание INT_SYS:
  SYS->INT.MBIT.IEA = 1; // SYS_IEA=1
  // включаем прерывание в модуле NVIC:
  SET_BIT(CPU->ISER, 1); // SETENA 1
}
*/

void rtc_write_unlock() {
  RH(RTC_KEY_h0) = 0xA217; // unlock access to regs
}


void rtc_write_lock() {
  RH(RTC_KEY_h0) = 0; // lock access to regs
}


void rtc_set_out(uint8_t out_mode) {
  RH(RTC_KEY_h0) = 0xA217; // unlock access to regs
  RB(RTC_CR0_b1) = out_mode; // RTC_OUT mode
  RH(RTC_KEY_h0) = 0; // lock access to regs
}
