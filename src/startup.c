/*
  MG32F02 startup.c by reug@mail.ru
  (Based on LPC804 startup.c by olikraus@gmail.com)
*/

typedef void (*isr_handler_t)(void);

#define Reserved_Handler ((isr_handler_t)0xFFFFFFFF)


#include "init.h"


int __attribute__ ((noinline)) main(void);


/*=======================================================================*/
/*
  Reserve some space for the stack. This is used to check if global variables + stack exceed RAM size.
  If -Wl,--gc-sections is used, then also define -Wl,--undefined=arm_stack_area to keep the variable in RAM.
  The name of the variable (here: arm_stack_area) does not matter.

  Heap (=dynamic memory allocation) is not supported
*/
#ifndef __STACK_SIZE
#define __STACK_SIZE 0x100
#endif
unsigned char arm_stack_area[__STACK_SIZE] __attribute__ ((section(".stack"))) __attribute__ ((aligned(8)));

/* make the top of the stack known to the c compiler, value will be calculated by the linker script */
void __StackTop(void);


/* the default handler is only referenced via alias attribute */
void __attribute__ ((interrupt)) Default_Handler(void) {
}


void __attribute__ ((interrupt)) __attribute__ ((noreturn)) Reset_Handler(void) {
  init_clock();
  main();
  while (1);
}

/* This is a weak function and can be overwritten by the user */
void __attribute__ ((interrupt, weak)) NMI_Handler(void) {
}


/* This is a weak function and can be overwritten by the user */
void __attribute__ ((interrupt, weak)) HardFault_Handler(void) {
}


void __attribute__ ((interrupt, weak, alias("Default_Handler"))) SVC_Handler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) PendSV_Handler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) ();


// Peripheral IRQ handlers

void __attribute__ ((interrupt, weak, alias("Default_Handler"))) WWDT_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) SYS_IRQHandler();

void __attribute__ ((interrupt, weak, alias("Default_Handler"))) EXINT0_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) EXINT1_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) EXINT2_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) EXINT3_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) COMP_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) DMA_IRQHandler();

void __attribute__ ((interrupt, weak, alias("Default_Handler"))) ADC_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) DAC_IRQHandler();

void __attribute__ ((interrupt, weak, alias("Default_Handler"))) TM0x_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) TM10_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) TM1x_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) TM20_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) TM2x_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) TM3x_IRQHandler();

void __attribute__ ((interrupt, weak, alias("Default_Handler"))) URT0_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) URT123_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) URT4x_IRQHandler();

void __attribute__ ((interrupt, weak, alias("Default_Handler"))) SPI0_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) SPIx_IRQHandler();

void __attribute__ ((interrupt, weak, alias("Default_Handler"))) I2C0_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) I2Cx_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) USB_IRQHandler();
void __attribute__ ((interrupt, weak, alias("Default_Handler"))) APX_IRQHandler();


// ISR vectors

isr_handler_t
__isr_vector[48] __attribute__ ((section(".isr_vector"))) __attribute__ ((aligned(4))) =
{
  // ARM Exceptions
  __StackTop,           // 0x00: Top of Stack, calculated by the linker script
  Reset_Handler,		    // 0x04: Reset Handler
  NMI_Handler,			    // 0x08: NMI Handler
  HardFault_Handler,    // 0x0c: Hard Fault Handler
  Reserved_Handler,     // 0x10: Reserved
  Reserved_Handler,     // 0x14: Reserved
  Reserved_Handler,     // 0x18: Reserved
  Reserved_Handler,     // 0x1c: Reserved
  Reserved_Handler,     // 0x20: Reserved
  Reserved_Handler,     // 0x24: Reserved
  Reserved_Handler,     // 0x28: Reserved
  SVC_Handler,          // 0x2c: SVCall Handler
  Reserved_Handler,     // 0x30: Reserved
  Reserved_Handler,     // 0x34: Reserved
  PendSV_Handler,       // 0x38: PendSV Handler
  SysTick_Handler,      // 0x3c: SysTick Handler

  // Peripheral IRQs
  WWDT_IRQHandler,      // 0x40: Window Watchdog
  SYS_IRQHandler,       // 0x44: System global Interrupt
  Reserved_Handler,     // 0x48: Reserved
  EXINT0_IRQHandler,    // 0x4c: EXIC EXINT0
  EXINT1_IRQHandler,    // 0x50: EXIC EXINT1
  EXINT2_IRQHandler,    // 0x54: EXIC EXINT2
  EXINT3_IRQHandler,    // 0x58: EXIC EXINT3
  COMP_IRQHandler,      // 0x5c: Analog Comparators global Interrupt
  DMA_IRQHandler,       // 0x60: DMA all channel global Interrupt
  Reserved_Handler,     // 0x64: Reserved
  ADC_IRQHandler,       // 0x68: ADC global Interrupt
  DAC_IRQHandler,       // 0x6c: DAC global Interrupt
  TM0x_IRQHandler,      // 0x70: Timer TM0x global Interrupt
  TM10_IRQHandler,      // 0x74: Timer TM10
  TM1x_IRQHandler,      // 0x78: Timer TM1x global Interrupt
  TM20_IRQHandler,      // 0x7c: Timer TM20
  TM2x_IRQHandler,      // 0x80: Timer TM2x global Interrupt
  TM3x_IRQHandler,      // 0x84: Timer TM3x global Interrupt
  Reserved_Handler,     // 0x88: Reserved
  Reserved_Handler,     // 0x8c: Reserved
  URT0_IRQHandler,      // 0x90: UART URT0
  URT123_IRQHandler,    // 0x94: UART URT1/2/3 global Interrupt
  URT4x_IRQHandler,     // 0x98: UART URT4/5/6/7/.. global Interrupt
  Reserved_Handler,     // 0x9c: Reserved
  SPI0_IRQHandler,      // 0xa0: SPI0
  SPIx_IRQHandler,      // 0xa4: SPIx global Interrupt
  Reserved_Handler,     // 0xa8: Reserved
  Reserved_Handler,     // 0xac: Reserved
  I2C0_IRQHandler,      // 0xb0: I2C0
  I2Cx_IRQHandler,      // 0xb4: I2Cx global Interrupt
  USB_IRQHandler,       // 0xb8: USB
  APX_IRQHandler        // 0xbc: APX

};

