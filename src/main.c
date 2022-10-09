#include "MG32x02z__RegAddress.h"
#include "api.h"
#include "ulib.h"
#include "init.h"
#include "core.h"
#include "hwcf.h" // Аппаратная конфигурация


/// Heartbeat
void heartbeat() {
  RH(HW_LED1_CRH0) = 0x0002; // PB13 -> push-pull output
  while (1) {
    RH(HW_LED1_SCH0) = HW_LED1_MASK; // set bit 2
    delay_ms(250);
    RH(HW_LED1_SCH1) = HW_LED1_MASK; // clear bit 2
    delay_ms(250);
  }
}


__attribute__ ((noreturn))  __attribute__ ((naked)) // omit prologue/epilogue sequences (garbage push/pop instructions)
void main (void) {
  init_clock();
//  setup_icko();
//
//  if (!setup_xosc()) {
//    *(volatile uint16_t*)PB_CR14_h0 = 0x0002; // PB14 -> push-pull output
//    *(volatile uint16_t*)PB_SC_h0 = (1 << 14);  // red light
//  }
//
//  heartbeat();

  if (*((volatile uint32_t*)(APP_ORIGIN)) == APP_SIGNATURE) {
    asm("BX %0" : : "r"((APP_ORIGIN+4) | 1)); // Set bit 0 for Thumb !!!
  }

  RH(HW_LED2_CRH0) = 0x0002; // PB14 -> push-pull output
  while (1) {
    RH(HW_LED2_SCH0) = HW_LED2_MASK; // set bit 14
    delay_ms(100);
    RH(HW_LED2_SCH1) = HW_LED2_MASK; // clear bit 14
    delay_ms(900);
  }

}
