#include "timer.h"
#include "MG32x02z__RegAddress.h"
#include "MG32x02z_CSC.h"
#include "MG32x02z_CPU.h"
#include "MG32x02z_TM.h"
#include "core.h"


void tm00_init() {
  RH(CSC_KEY_h0) = 0xA217; // unlock access to CSC regs
  RB(CSC_APB1_h0) |= CSC_APB1_TM00_EN_enable_h0;
  RH(CSC_KEY_h0) = 0; // lock access to CSC regs
}


void tm00_set_int(uint8_t flags) {
  RB(TM00_INT_b0) = flags; // включаем прерывания в модуле
  // включаем прерывание в модуле NVIC:
  RW(CPU_ISER_w) = (1 << 12); // SETENA 12
}

