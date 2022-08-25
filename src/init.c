#include "init.h"
#include "MG32x02z__RegAddress.h"
#include "MG32x02z_CSC.h"


void init_clock() {
  *((volatile uint16_t*)CSC_KEY_h0) = 0xA217; // unlock access to CSC regs

  // Setup CSC for PB and PC
  *((volatile uint32_t*)CSC_AHB_w) |=
    CSC_AHB_IOPB_EN_enable_w | // CSC_IOPB_EN = 1
    CSC_AHB_IOPC_EN_enable_w | // CSC_IOPC_EN = 1
    CSC_AHB_IOPD_EN_enable_w;   // CSC_IOPD_EN = 1

  *((volatile uint16_t*)CSC_KEY_h0) = 0; // lock access to CSC regs
}


// CK_ICKO output through PC0 pin
void setup_icko() {
  *(volatile uint16_t*)PC_CR0_h0 = (0x1 << 12) | 2; // PC0 -> ICKO, push pull output
  *(volatile uint16_t*)CSC_KEY_h0 = 0xA217; // unlock access to CSC regs
  //*(volatile uint32_t*)CSC_CKO_w = (0x0 << 4) | 1; // CK_MAIN, DIV=1, CSC_CKO_EN = 1
  //*(volatile uint32_t*)CSC_CKO_w = (0x3 << 4) | 1; // CK_HS, DIV=1, CSC_CKO_EN = 1
  //*(volatile uint32_t*)CSC_CKO_w = (0x0 << 4) | (2 << 2) | 1; // CK_MAIN, DIV=4, CSC_CKO_EN = 1
  *(volatile uint8_t*)CSC_CKO_w =
    //CSC_CKO_CKO_SEL_ck_ls_b0 | // CK_LS output
    CSC_CKO_CKO_SEL_ck_hs_b0 | // CK_HS output
    //CSC_CKO_CKO_SEL_ck_xosc_b0 | // CK_HS output
    CSC_CKO_CKO_DIV_div4_b0 |  // DIV8
    CSC_CKO_CKO_EN_enable_b0;


  (0x0 << 4) | (2 << 2) | 1; // CK_MAIN, DIV=4, CSC_CKO_EN = 1
  *(volatile uint16_t*)CSC_KEY_h0 = 0x1111; // lock access to CSC regs
}


// Установка частоты IHRCO на 11,0592 МГц
void setup_ihrco() {
  *(volatile uint16_t*)CSC_KEY_h0 = 0xA217; // unlock access to CSC regs
  *(volatile uint32_t*)CSC_CR0_w |= (1 << 18); // CSC_IHRCO_SEL = 1 (11.0592 MHz)
  *(volatile uint16_t*)CSC_KEY_h0 = 0x1111; // lock access to CSC regs
}


void csc_xosc_init() {
  //Gain control bits of XOSC.
  *(volatile uint16_t*)CSC_KEY_h0 = 0xA217; // unlock access to CSC regs
  *(volatile uint16_t*)CSC_PLL_h1 = CSC_PLL_XOSC_GN_medium_h1; // CSC_XOSC_GN = 1
  *(volatile uint16_t*)CSC_KEY_h0 = 0; // lock access to CSC regs

  // Setup pins XIN (PC13) & XOUT (PC14):
  *(volatile uint16_t*)PC_CR13_h0 = (1 << 12); // PC13 -> XIN
  *(volatile uint16_t*)PC_CR14_h0 = (1 << 12); // PC14 -> XOUT
}


void csc_xosc_select() {
  uint32_t d;
  *(volatile uint16_t*)CSC_KEY_h0 = 0xA217; // unlock access to CSC regs
  d=*(volatile uint32_t*)CSC_CR0_w;
  d &= ~CSC_CR0_HS_SEL_mask_w; // clear bits 10,11
  d |=  CSC_CR0_HS_SEL_xosc_w; // set CSC_HS_SEL = 0b01 (XOSC)
  *(volatile uint32_t*)CSC_CR0_w = d;
  *(volatile uint16_t*)CSC_KEY_h0 = 0; // lock access to CSC regs
}


// Включение XOSC генератора.
uint8_t setup_xosc() {
  uint32_t d;
  csc_xosc_init();
  d=1000;
  // waiting CSC_XOSC_STA == 1
  while (! ( *(volatile uint32_t*)CSC_STA_b3 & CSC_STA_XOSC_STA_ready_b3) ) {
    if (--d==0) return 0;
  }
  csc_xosc_select();
  return 1;
}


// Включение умножения частоты на основе PLL (x2)
void setup_pll() {
  uint16_t d;
  *(volatile uint16_t*)CSC_KEY_h0 = 0xA217; // unlock access to CSC regs

  // CSC_PLLI_DIV = 2, CK_PLLI = 6 MHz (CK_HS/2)
  // CSC_PLLO_DIV = 2 , CK_PLLO = 48 MHz (CK_PLL/2)
  //*(volatile uint8_t*)CSC_DIV_b0 = 0b00100001; // CK_PLLO = 48 MHz (CK_PLL/2)
  *(volatile uint8_t*)CSC_DIV_b0 = 0b00000001; // CK_PLLO = 24 MHz (CK_PLL/2)

  // CSC_PLL.CSC_PLL_MUL = 0, PLL_MULL = 16 (DEFAULT), CK_PLL = 96 MHz (CK_PLII*16)
  //*(volatile uint8_t*)CSC_PLL_b1 |= 1; // CSC_PLL_MUL = 1, PLL_MULL = 24, CK_PLL = 144 MHz (CK_PLII*24)

  *(volatile uint8_t*)CSC_CR0_b0 |= (1 << 5); // CSC_PLL_EN = 1

  while (! (*(volatile uint8_t*)CSC_STA_b0 & (1 << 6))); // waiting CSC_PLLF == 1 (PLL ready)

  d=*(volatile uint16_t*)CSC_CR0_h0;
  d &= ~(3 << 14); // clear bits 14,15
  d |= (2 << 14); // CSC_MAIN_SEL = 2 (CK_PLLO)
  *(volatile uint16_t*)CSC_CR0_h0 = d;

  *(volatile uint16_t*)CSC_KEY_h0 = 0x1111; // lock access to CSC regs
}


void csc_set_ck_ut() {
  *(volatile uint16_t*)CSC_KEY_h0 = 0xA217; // unlock access to CSC regs
  *(volatile uint8_t*)CSC_DIV_b3 = CSC_DIV_UT_DIV_div8_b3; // CSC_UT_DIV
  *(volatile uint16_t*)CSC_KEY_h0 = 0; // lock access to CSC regs
}
