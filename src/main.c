#include "MG32x02z__RegAddress.h"
#include "api.h"
#include "ulib.h"


__attribute__ ((noreturn))  __attribute__ ((naked)) // omit prologue/epilogue sequences (garbage push/pop instructions)
void main (void) {

  if (*((volatile uint32_t*)(APP_ORIGIN)) == APP_SIGNATURE) {
    asm("BX %0" : : "r"((APP_ORIGIN+4) | 1)); // Set bit 0 for Thumb !!!
  }

  *(volatile uint16_t*)PB_CR3_h0 = 0x0002; // PB3 -> push-pull output
  while (1) {
    *(volatile uint16_t*)PB_SC_h0 = 0x0008; // set bit 3
    delay_ms(100);
    *(volatile uint16_t*)PB_SC_h1 = 0x0008; // clear bit 3
    delay_ms(900);
  }

}
