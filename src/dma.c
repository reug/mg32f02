#include "dma.h"
#include "MG32x02z_CSC.h"
#include "MG32x02z_DMA.h"
#include "core.h"


void dma_init() {
  RH(CSC_KEY_h0) = 0xA217; // unlock access to CSC regs
  RH(CSC_AHB_h0) |= CSC_AHB_DMA_EN_enable_h0;
  RH(CSC_KEY_h0) = 0; // lock access to CSC regs
  //RB(DMA_CR0_b0) = DMA_CR0_PRI_MDS_level_b0 | DMA_CR0_EN_enable_b0;
  RB(DMA_CR0_b0) = DMA_CR0_EN_enable_b0;
}


void dma_setup(uint8_t ch, uint32_t flags) {
  uint32_t ba=DMA_Base + 0x20*(ch+1); // DMA_CHnA
  //RW(ba) = flags | DMA_CH0A_CH0_PLS_lv3_w | DMA_CH0A_CH0_EN_enable_w;
  RW(ba) = flags | DMA_CH0A_CH0_EN_enable_w;
}


void dma_setup_sd(uint8_t ch, uint16_t srcdet) {
  uint32_t ba=DMA_Base + 0x20*(ch+1);
  RH(ba + DMA_CH0B_w - DMA_CH0A_w) = srcdet;
  //RW(ba + DMA_CH0B_w - DMA_CH0A_w) = DMA_CH0B_CH0_DSYNC_enable_w | DMA_CH0B_CH0_SSYNC_enable_w | srcdet;
}


void dma_setup_memsrc(uint8_t ch, void* addr) {
  uint32_t ba=DMA_Base + 0x20*(ch+1);
  RW(ba + DMA_CH0SSA_w - DMA_CH0A_w) = (uint32_t)addr;
}


void dma_setup_memdst(uint8_t ch, void* addr) {
  uint32_t ba=DMA_Base + 0x20*(ch+1);
  RW(ba + DMA_CH0DSA_w - DMA_CH0A_w) = (uint32_t)addr;
}


void dma_setup_amount(uint8_t ch, uint32_t n) {
  uint32_t ba=DMA_Base + 0x20*(ch+1);
  RW(ba + DMA_CH0NUM_w - DMA_CH0A_w) = n;
}


void dma_start(uint8_t ch, uint8_t chna_b1) {
  uint32_t ba=DMA_Base + 0x20*(ch+1);
  RB(ba + 1) = chna_b1 | DMA_CH0A_CH0_REQ_enable_b1;
}


void dma_setup_int(uint8_t ch, uint8_t flags) {
  uint32_t ba=DMA_Base;
  RB(ba + DMA_INT_b0 - DMA_Base) = DMA_INT_IEA_enable_b0;
  ba += 0x20*(ch+1); // set base address to DMA_CHnA
  RB(ba) = flags;
}
