#include "wdt.h"
#include "MG32x02z__RegAddress.h"
#include "MG32x02z_CSC.h"
#include "MG32x02z_IWDT.h"
#include "MG32x02z_WWDT.h"
#include "MG32x02z_SYS.h"
#include "MG32x02z_CPU.h"
#include "core.h"


void iwdt_init() {
  RH(CSC_KEY_h0) = 0xA217; // unlock access to CSC regs
  RB(CSC_APB0_b0) |= CSC_APB0_IWDT_EN_enable_b0; // CSC_IWDT_EN = 1
  RH(CSC_KEY_h0) = 0; // lock access to CSC regs
}


void iwdt_reload() {
  RH(IWDT_KEY_h0) = 0x2014;
}


void iwdt_write_unlock() {
  RH(IWDT_KEY_h0) = 0xA217; // unlock access to regs
}


void iwdt_write_lock() {
  RH(IWDT_KEY_h0) = 0; // lock access to regs
}


void iwdt_set_int(uint8_t flags) {
  RH(IWDT_KEY_h0) = 0xA217; // unlock access to regs
  RB(IWDT_INT_b0) = flags; // включаем прерывания в модуле
  RH(IWDT_KEY_h0) = 0; // lock access to regs
  // включаем прерывание INT_SYS:
  RB(SYS_INT_b0) = 1; // SYS_IEA=1
  // включаем прерывание в модуле NVIC:
  RW(CPU_ISER_w) = (1 << 1); // SETENA 1
}


void wwdt_init() {
  RH(CSC_KEY_h0) = 0xA217; // unlock access to CSC regs
  RB(CSC_APB0_b0) |= CSC_APB0_WWDT_EN_enable_b0; // CSC_WWDT_EN = 1
  RH(CSC_KEY_h0) = 0; // lock access to CSC regs
}


void wwdt_reload() {
  RH(WWDT_KEY_h0) = 0x2014;
}


void wwdt_write_unlock() {
  RH(WWDT_KEY_h0) = 0xA217; // unlock access to regs
}


void wwdt_write_lock() {
  RH(WWDT_KEY_h0) = 0; // lock access to regs
}


void wwdt_set_int(uint8_t flags) {
  RH(WWDT_KEY_h0) = 0xA217; // unlock access to regs
  RB(WWDT_INT_b0) = flags; // включаем прерывания в модуле
  RH(WWDT_KEY_h0) = 0; // lock access to regs
  // включаем прерывание в модуле NVIC:
  RW(CPU_ISER_w) = 1; // SETENA 0
}

