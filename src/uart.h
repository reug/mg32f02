#ifndef UART_H
#define UART_H

#include <stdint.h>


#define UART_NEWLINE_NONE   0x00000000
#define UART_NEWLINE_LF     0x0000000A
#define UART_NEWLINE_CR     0x0000000D
#define UART_NEWLINE_CRLF   0x00000A0D
#define UART_NEWLINE_LFCR   0x00000D0A
#define UART_NEWLINE_CRLF2  0x0A0D0A0D
#define UART_NEWLINE_LFCR2  0x0D0A0D0A

/// Инициализация UART
void uart_init(uint8_t port_no);

/// Блокирующая отправка одного байта
void uart_tx(uint8_t port_no, uint8_t d);

/// Блокирующая отправка данных из буфера
void uart_send(uint8_t port_no, const void* buf, uint32_t len);

/// Блокирующая отправка Си-строки с опциональными завершающими символами
void uart_puts(uint8_t port_no, const char* s, uint32_t newline);

/// Блокирующее чтение одного байта
uint8_t uart_rx(uint8_t port_no);


#endif
