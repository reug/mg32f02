#include "timer.h"
#include "MG32x02z__RegAddress.h"
#include "MG32x02z_CSC.h"
#include "MG32x02z_CPU.h"
#include "MG32x02z_TM.h"
#include "core.h"


// Base addresses
const uint32_t TM_BASE[8]={
  TM00_Base, TM01_Base,
  TM10_Base, TM16_Base,
  TM20_Base, TM26_Base,
  TM36_Base, 0xffffffff
};


// Bit mask for CSC_APB1_h0
const uint16_t TM_CSC[8]={
  CSC_APB1_TM00_EN_mask_h0, CSC_APB1_TM01_EN_mask_h0,
  CSC_APB1_TM10_EN_mask_h0, CSC_APB1_TM16_EN_mask_h0,
  CSC_APB1_TM20_EN_mask_h0, CSC_APB1_TM26_EN_mask_h0,
  CSC_APB1_TM36_EN_mask_h0, 0xffff
};


// IRQs
const uint8_t TM_IRQ[8]={12, 12, 13, 14, 15, 16, 17, 0xff};


void tm_init(uint8_t tm_id) {
  RH(CSC_KEY_h0) = 0xA217; // unlock access to CSC regs
  RH(CSC_APB1_h0) |= TM_CSC[tm_id];
  RH(CSC_KEY_h0) = 0; // lock access to CSC regs
}


void tm_setup_int(uint8_t tm_id, uint32_t flags) {
  RW(TM_BASE[tm_id]+TM00_INT_w-TM00_STA_w) = flags | TM_INT_IEA_enable_w; // включаем прерывания в модуле
  // включаем прерывание в модуле NVIC:
  RW(CPU_ISER_w) = (1 << TM_IRQ[tm_id]); // SETENA
}

