#include "rtc.h"
#include "MG32x02z__RegAddress.h"
#include "MG32x02z_CSC.h"
#include "MG32x02z_RTC.h"


void rtc_init() {
  *(volatile uint16_t*)CSC_KEY_h0 = 0xA217; // unlock access to CSC regs
  *(volatile uint8_t*)CSC_APB0_b0 |= CSC_APB0_RTC_EN_enable_b0; // CSC_RTC_EN = 1
  *(volatile uint16_t*)CSC_KEY_h0 = 0; // lock access to CSC regs
}


void rtc_set_int(uint8_t flags) {
  *(volatile uint16_t*)RTC_KEY_h0 = 0xA217; // unlock access to regs
  // включаем прерывания в модуле:
  *(volatile uint8_t*)RTC_INT_b0 = flags | RTC_INT_IEA_enable_b0; // RTC_IEA
  *(volatile uint16_t*)RTC_KEY_h0 = 0; // lock access to regs
  // включаем прерывание INT_SYS:
  *(volatile uint8_t*)SYS_INT_b0 = 1; // SYS_IEA=1
  // включаем прерывание в модуле NVIC:
  *(volatile uint32_t*)CPU_ISER_w = (1 << 1); // SETENA 1
}


void rtc_write_unlock() {
  *(volatile uint16_t*)RTC_KEY_h0 = 0xA217; // unlock access to regs
}


void rtc_write_lock() {
  *(volatile uint16_t*)RTC_KEY_h0 = 0; // lock access to regs
}


void rtc_out(uint8_t out_sel) {
  *(volatile uint16_t*)RTC_KEY_h0 = 0xA217; // unlock access to regs
  // включаем прерывания в модуле:
  *(volatile uint8_t*)RTC_CR0_b1 |= out_sel; // RTC_OUT_SEL
  *(volatile uint16_t*)RTC_KEY_h0 = 0; // lock access to regs
}
