#include "init.h"
#include "MG32x02z__RegAddress.h"


void init_clock() {
  *((volatile uint16_t*)CSC_KEY_h0) = 0xA217; // unlock access to CSC regs

  // Setup CSC for PB
  *((volatile uint32_t*)CSC_AHB_w) |= 2 | 4; // CSC_IOPB_EN = 1, CSC_IOPC_EN = 1

  *((volatile uint16_t*)CSC_KEY_h0) = 0x1111; // lock access to CSC regs
}
