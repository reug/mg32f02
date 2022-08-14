#include "MG32x02z__RegAddress.h"
#include "api.h"
#include "ulib.h"
#include "init.h"


/// Heartbeat
void heartbeat() {
  *(volatile uint16_t*)PB_CR13_h0 = 0x0002; // PB13 -> push-pull output
  while (1) {
    *(volatile uint16_t*)PB_SC_h0 = (1 << 13); // set bit 2
    delay_ms(250);
    *(volatile uint16_t*)PB_SC_h1 = (1 << 13); // clear bit 2
    delay_ms(250);
  }
}


__attribute__ ((noreturn))  __attribute__ ((naked)) // omit prologue/epilogue sequences (garbage push/pop instructions)
void main (void) {
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

  *(volatile uint16_t*)PB_CR14_h0 = 0x0002; // PB14 -> push-pull output
  while (1) {
    *(volatile uint16_t*)PB_SC_h0 = (1 << 14); // set bit 14
    delay_ms(100);
    *(volatile uint16_t*)PB_SC_h1 = (1 << 14); // clear bit 14
    delay_ms(900);
  }

}
