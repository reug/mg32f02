#include "cmp.h"
#include "MG32x02z__RegAddress.h"


void cmp_init() {
  //uint32_t cr=no ? CMP_CR0_w : CMP_CR1_w;

  *((volatile uint16_t*)CSC_KEY_h0) = 0xA217; // unlock access to CSC regs
  *(volatile uint8_t*)CSC_APB0_b0 |= 4; // CSC_CMP_EN = 1
  *((volatile uint16_t*)CSC_KEY_h0) = 0x1111; // lock access to CSC regs

  //*(volatile uint8_t*)cr = 1;// CMP_AC0_EN = 1
}

