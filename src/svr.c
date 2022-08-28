#include "api.h"
#include "ulib.h"
#include "MG32x02z__RegAddress.h"
#include "core.h"


/// IRQ Handler type
typedef void(*handler_t)();


/// Core handlers
volatile handler_t hdlr_systick;

/// IRQ Handlers
volatile handler_t hdlr[32];

/*
// MG32F02A032
__attribute__ ((naked))
void HardFault_Handler() {
  // Включаем мигание светодиодом на PB3
  *(volatile uint16_t*)PB_CR3_h0 = 0x0002; // PB3 -> push-pull output
  while (1) {
    *(volatile uint16_t*)PB_SC_h0 = 0x0008; // set bit 3
    delay_ms(100);
    *(volatile uint16_t*)PB_SC_h1 = 0x0008; // clear bit 3
    delay_ms(100);
  }
}
*/


// Cortex-M0 Exceptions:

// MG32F02A064 LQFP48
__attribute__ ((naked))
void HardFault_Handler() {
  // Включаем мигание светодиодом на PB14
  RH(PB_CR14_h0) = 0x0002; // PB14 -> push-pull output
  while (1) {
    RH(PB_SC_h0) = (1 << 14); // set bit 14
    delay_ms(100);
    RH(PB_SC_h1) = (1 << 14); // clear bit 14
    delay_ms(100);
  }
}



__attribute__ ((naked))
void SVC_Handler() {
// From Yiu J.:
// Stack frame contains:
// r0, r1, r2, r3, r12, r14, the return address and xPSR
// - Stacked R0 = svc_args[0]
// - Stacked R1 = svc_args[1]
// - Stacked R2 = svc_args[2]
// - Stacked R3 = svc_args[3]
// - Stacked R12 = svc_args[4]
// - Stacked LR = svc_args[5]
// - Stacked PC = svc_args[6]
// - Stacked xPSR= svc_args[7]
  // Используем только MSP, проверку бита 2 LR опускаем
  asm(
    "mrs    r0,msp\n"
    "b      SVC_Handler_main\n"
  );
}


/// Установка обработчика прерываний
__attribute__ ((interrupt))
void SVC_Handler_main(uint32_t* sp) {
  switch ( ((uint8_t*)sp[6])[-2] ) {
    case SVC_HANDLER_UNSET:   hdlr[sp[0]]=0; break;
    case SVC_HANDLER_SET:     hdlr[sp[0]]=sp[1]; break;
    case SVC_CHANDLER_UNSET:  break; // TODO
    case SVC_CHANDLER_SET:
      switch (sp[0]) {
        case 15: hdlr_systick=sp[1]; break;
      }
      break;
  }
}


__attribute__ ((interrupt))
void SysTick_Handler() {
  if (hdlr_systick) hdlr_systick();
}


// IRQ interrupts:

__attribute__ ((interrupt))
void WWDT_IRQHandler() {
  if (hdlr[0]) hdlr[0]();
}

__attribute__ ((interrupt))
void SYS_IRQHandler() {
  if (hdlr[1]) hdlr[1]();
}

__attribute__ ((interrupt))
void ADC_IRQHandler() {
  if (hdlr[10]) hdlr[10]();
}

__attribute__ ((interrupt))
void URT0_IRQHandler() {
  if (hdlr[20]) hdlr[20]();
}

__attribute__ ((interrupt))
void URT123_IRQHandler() {
  if (hdlr[21]) hdlr[21]();
}
