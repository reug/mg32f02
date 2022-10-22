#include "api.h"
#include "ulib.h"
#include "core.h"
#include "hwcf.h" // Аппаратная конфигурация


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

__attribute__ ((naked))
void HardFault_Handler() {
  // Включаем мигание красным светодиодом D2
  RH(HW_LED2_CRH0) = 0x0002; // pin -> push-pull output
  while (1) {
    RH(HW_LED2_SCH0) = HW_LED2_MASK; // set bit 14
    delay_ms(100);
    RH(HW_LED2_SCH1) = HW_LED2_MASK; // clear bit 14
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
void EXINT0_IRQHandler() {
  if (hdlr[3]) hdlr[3]();
}

__attribute__ ((interrupt))
void EXINT1_IRQHandler() {
  if (hdlr[4]) hdlr[4]();
}

__attribute__ ((interrupt))
void EXINT2_IRQHandler() {
  if (hdlr[5]) hdlr[5]();
}

__attribute__ ((interrupt))
void EXINT3_IRQHandler() {
  if (hdlr[6]) hdlr[6]();
}

__attribute__ ((interrupt))
void ADC_IRQHandler() {
  if (hdlr[10]) hdlr[10]();
}

__attribute__ ((interrupt))
void DAC_IRQHandler() {
  if (hdlr[11]) hdlr[11]();
}

__attribute__ ((interrupt))
void TM0x_IRQHandler() {
  if (hdlr[12]) hdlr[12]();
}

__attribute__ ((interrupt))
void TM10_IRQHandler() {
  if (hdlr[13]) hdlr[13]();
}

__attribute__ ((interrupt))
void TM1x_IRQHandler() {
  if (hdlr[14]) hdlr[14]();
}

__attribute__ ((interrupt))
void TM20_IRQHandler() {
  if (hdlr[15]) hdlr[15]();
}

__attribute__ ((interrupt))
void TM2x_IRQHandler() {
  if (hdlr[16]) hdlr[16]();
}

__attribute__ ((interrupt))
void TM3x_IRQHandler() {
  if (hdlr[17]) hdlr[17]();
}

__attribute__ ((interrupt))
void URT0_IRQHandler() {
  if (hdlr[20]) hdlr[20]();
}

__attribute__ ((interrupt))
void URT123_IRQHandler() {
  if (hdlr[21]) hdlr[21]();
}

__attribute__ ((interrupt))
void SPI0_IRQHandler() {
  if (hdlr[24]) hdlr[24]();
}

__attribute__ ((interrupt))
void I2C0_IRQHandler() {
  if (hdlr[28]) hdlr[28]();
}

__attribute__ ((interrupt))
void I2C1_IRQHandler() {
  if (hdlr[29]) hdlr[29]();
}
