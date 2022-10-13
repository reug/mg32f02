#ifndef ULIB_H
#define ULIB_H

#include <stdint.h>
#include <stddef.h>


/// libc memcpy replacement
void *memcpy(void *dest, const void *src, size_t n);

/// libc memset replacement
void *memset(void *dest, int c, size_t n);

/// Simple loop delay
void delay(uint32_t span);

/// Double-Dabble algorithm
/// Get 5-digit 20-bit BCD value from 16-bit integer
uint32_t bcd16(uint32_t b);

/// Множитель для delay (мс) для частоты 12000 кГц
#define DELAY_KMS   2260
#define delay_ms(x) delay((x)*DELAY_KMS)

/// Возвращает hex-цифру.
char hexdigit(uint8_t d);

///// String (dec) from unsigned 8-bit integer with zero padding
//char* strUint8(char* str, uint8_t width, uint8_t num);

///// String (hex) from 8-bit integer with zero padding
///// str[] should be at least 3 bytes
//char* strUint8hex(char* str, uint16_t num);

/// String (dec) from unsigned 16-bit integer with zero padding
char* strUint16(char* str, uint8_t width, uint16_t num);

/// String (hex) from 16-bit integer with zero padding
/// str[] should be at least 5 bytes
char* strUint16hex(char* str, uint16_t num);

/// String (dec) from 32-bit integer with zero padding
/// str[] should be at least (width+1) bytes
char* strUint32(char* str, uint8_t width, uint32_t num);

/// String (hex) from 32-bit integer with zero padding
/// str[] should be at least 9 bytes
char* strUint32hex(char* str, uint32_t num);

/// Division
int32_t divs32(int32_t Dividend, int32_t Divisor);
int32_t mods32(int32_t Dividend, int32_t Divisor);
uint32_t divu32(uint32_t Dividend, uint32_t Divisor);
uint32_t modu32(uint32_t Dividend, uint32_t Divisor);

#endif
