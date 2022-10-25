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

void debug16hex(uint16_t h) {
  uint32_t d;
  d=
    hexdigit(h >> 12) |
    (hexdigit((h >> 8) & 0x0F)  << 8) |
    (hexdigit((h >> 4) & 0x0F) << 16) |
    (hexdigit(h & 0x0F) << 24);
  uart_send4(PORT,d);
  uart_send4(PORT,UART_NEWLINE_CRLF);
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

void debugbuf(uint8_t* buf, uint32_t len) {
  uint32_t i;
  for (i=0; i<len; i++) {
    uart_tx(PORT,hexdigit(buf[i] >> 4));
    uart_tx(PORT,hexdigit(buf[i] & 0x0F));
    uart_tx(PORT,' ');
  }
  uart_puts(PORT, "", UART_NEWLINE_CRLF);
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
