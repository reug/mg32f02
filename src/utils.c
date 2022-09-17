#include "utils.h"
#include "ulib.h"
#include "uart.h"
#include "core.h"


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
  *(volatile uint16_t*)PB_SC_h0 = (1 << 13); // set bit 13
  delay_ms(100);
  *(volatile uint16_t*)PB_SC_h1 = (1 << 13); // clear bit 13
  delay_ms(100);
}

void led2_flash() {
  *(volatile uint16_t*)PB_SC_h0 = (1 << 14); // set bit 14
  delay_ms(100);
  *(volatile uint16_t*)PB_SC_h1 = (1 << 14); // clear bit 14
  delay_ms(100);
}

void led_blink() {
  RH(PB_SC_h1) = (1 << 14); // clear bit 14
  RH(PB_SC_h0) = (1 << 13); // set bit 13
  delay_ms(250);
  RH(PB_SC_h1) = (1 << 13); // clear bit 13
  RH(PB_SC_h0) = (1 << 14); // set bit 14
  delay_ms(250);
}
