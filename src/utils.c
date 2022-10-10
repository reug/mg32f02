#include "utils.h"
#include "ulib.h"
#include "uart.h"
#include "core.h"
#include "hwcf.h"


void debug(char label, uint16_t d) {
  char s[32];
  s[0]=label; s[1]=' ';
  strUint16hex(s+2,d); s[6]=' ';
  strUint16(s+7,5,d);
  uart_puts(PORT, s, UART_NEWLINE_CRLF);
}

void debug32(char label, uint32_t d) {
  char s[16];
  s[0]=label; s[1]=' ';
  strUint32(s+2,10,d);
  uart_puts(PORT,s,UART_NEWLINE_CRLF);
}

void debug32hex(char label, uint32_t d) {
  char s[12];
  s[0]=label; s[1]=' ';
  strUint32hex(s+2,d); s[10]=0;
  uart_puts(PORT, s, UART_NEWLINE_CRLF);
}

void led1_flash() {
  RH(HW_LED1_SCH0) = HW_LED1_MASK;
  delay_ms(100);
  RH(HW_LED1_SCH1) = HW_LED1_MASK;
  //delay_ms(100);
}

void led2_flash() {
  RH(HW_LED2_SCH0) = HW_LED2_MASK;
  delay_ms(100);
  RH(HW_LED2_SCH1) = HW_LED2_MASK;
  //delay_ms(100);
}

void led_blink() {
  RH(HW_LED2_SCH1) = HW_LED2_MASK;
  RH(HW_LED1_SCH0) = HW_LED1_MASK;
  delay_ms(250);
  RH(HW_LED1_SCH1) = HW_LED1_MASK;
  RH(HW_LED2_SCH0) = HW_LED2_MASK;
  delay_ms(250);
}
