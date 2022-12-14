#include "ulib.h"
#include "cmsis_gcc.h"
#include "core.h"
#include "MG32x02z_GPL.h"
#include "MG32x02z_GPL_MID.h"


void *memcpy(void *dest, const void *src, size_t n) {
  size_t i;
  for (i=0; i<n; i++) ((uint8_t*)dest)[i]=((uint8_t*)src)[i]; // FIXME
  return dest;
}


void *memset(void *dest, int c, size_t n) {
  size_t i;
  for (i=0; i<n; i++) ((uint8_t*)dest)[i]=c;
  return dest;
}


void delay(uint32_t span) {
//  do __NOP(); while (--span);
  asm(
    "loop:  nop\n"
    "sub    r0,#1\n"
    "bne    loop\n"
  );
}


/// Double-Dabble algorithm
/// Get 5-digit 20-bit BCD value from 16-bit integer
//uint32_t bcd16(uint16_t b) {
//  int i,k;
//  uint64_t d=b;
//  for (i=0; i<16; i++) {
//    for (k=16; k<36; k+=4)
//      if ( ((d >> k) & 0xF) >= 5 ) d+= (3LL << k);
//    d <<= 1;
//  }
//  return (d >> 16);
//}

//#ifdef MG32F02A032
// Double-Dabble algorithm, assembler source by reug
__attribute__ ((noinline))
uint32_t bcd16(uint32_t b) {
  asm(
    "lsl r0,#16\n"    // r0 - исходное значение (b) (надо сразу сдвинуть на 16, если оно 16 битное)

    // Константы:
    "mov r1,#0\n"     // r1 - нибблы BCD (результат)
    "mov r3,#0xf\n"   // величина 0xF для AND
    "mov r4,#4\n"     // величина сдвига для ROR
    "mov r6,#16\n"    // r6 - число итераций внешнего цикла
    "mov r7,#12\n"    // величина "докрутки" r1 до 32 бит

    "shift:\n"
    // Цикл по нибблам (5 раз)
    "mov r5,#5\n"     // цикл по нибблам (5 раз, декремент в r5)

    "nibbles:\n"
    "mov r2,r1\n"     // tmp r1 -> r2
    "and r2,r3\n"     // r2 &= 0xf
    "cmp r2,#4\n"     // ниббл < 5 ?
    "bls noadd\n"     // не добавлем, если ниббл <5
    "add r1,r1,#3\n"  // r1+=3

    "noadd:\n"
    "ror r1,r4\n"     // ror r1,#4 - продвигаем в биты 0-3 r1 следующий ниббл
    "sub r5,#1\n"     // r5--
    "bne nibbles\n"

    "ror r1,r7\n"     // ror r1,#12 - докручиваем r1 до 32 бит, чтобы всё вернулось на свои места

    // Выполняем сдвиг всего влево
    "lsl r1,#1\n"
    "lsl r0,#1\n"
    "bcc nocarry\n"
    "add r1,#1\n"     // set bit 0 of r1 if carry
    "nocarry:"
    // цикл по сдвигу 16 раз
    "sub r6,#1\n"     // r6--
    "bne shift\n"
    "mov r0,r1\n"     // return r1
    : : : "r4","r5","r6","r7"
  );
}
//#endif

/*
char* strUint8(char* str, uint8_t width, uint8_t num) {
  str[width]=0;
  if (width>=1) str[width-1]=num%10+48;
  if (width>=2) str[width-2]=num/10%10+48;
  if (width>=3) str[width-3]=num/100+48;
  return str+width;
}


char* strUint8hex(char* str, uint16_t num) {
  uint8_t d=num;
  uint8_t c;
  int8_t k;
  for (k=1; k>=0; k--) {
    c=0x0F & d;
    str[k]=c + (c<10 ? 48 : 55);
    d = d >> 4;
  }
  str[2]=0;
  return str+2;
}
*/


char hexdigit(uint8_t d) {
  return d + (d < 10 ? 48 : 55);
}


char* strUint16(char* str, uint8_t width, uint16_t num) {
  str[width]=0;
  uint8_t i=width;
  uint32_t d=bcd16(num);
  do {
    str[--i] = (d & 0xf)+48;
    d >>= 4;
  } while (i);
  return str+width;
}


char* strUint16hex(char* str, uint16_t num) {
  uint8_t c;
  int8_t k;
  for (k=3; k>=0; k--) {
    c=0x000F & num;
    str[k]=c + (c<10 ? 48 : 55);
    num >>= 4;
  }
  str[4]=0;
  return str+4;
}


char* strUint32(char* str, uint8_t width, uint32_t num) {
  str[width]=0;
  uint32_t d=1;
  int8_t k;
  for (k=width-1; k>=0; k--) {
    str[k]= modu32(divu32(num,d),10) +48;
    d *= 10;
  }
  return str+width;
}


char* strUint32hex(char* str, uint32_t num) {
  uint8_t c;
  int8_t k;
  for (k=7; k>=0; k--) {
    c=0x0F & num;
    str[k]=c + (c<10 ? 48 : 55);
    num >>= 4;
  }
  str[8]=0;
  return str+8;
}


int32_t divs32(int32_t Dividend, int32_t Divisor) {
  return MID_GPL_HDIV_DIV(Dividend,Divisor);
}

int32_t mods32(int32_t Dividend, int32_t Divisor) {
  return MID_GPL_HDIV_MOD(Dividend,Divisor);
}

uint32_t divu32(uint32_t Dividend, uint32_t Divisor) {
  return MID_GPL_HDIV_UDIV(Dividend,Divisor);
}

uint32_t modu32(uint32_t Dividend, uint32_t Divisor) {
  return MID_GPL_HDIV_UMOD(Dividend,Divisor);
}

/*
void crc32_init(uint32_t val) {
  RB(GPL_CR1_b0)=0;
  RW(GPL_CRCINIT_w)=val;
  RW(GPL_DIN_w)=0;
  //RB(GPL_CR0_b0)= GPL_CR0_IN_INV_enable_b0; // GPL_CR0_BEND_EN_enable_b0;
  //RB(GPL_CR1_b0)= GPL_CR1_CRC_DSIZE_8bit_b0 | GPL_CR1_CRC_MDS_crc32_b0 | GPL_CR1_CRC_EN_enable_b0;
  RB(GPL_CR1_b0)= GPL_CR1_CRC_DSIZE_8bit_b0 | GPL_CR1_CRC_MDS_crc32_b0 | GPL_CR1_CRC_EN_enable_b0;
  //RB(GPL_CR1_b0)= GPL_CR1_CRC_BREV_32bit_b0 | GPL_CR1_CRC_DSIZE_8bit_b0 | GPL_CR1_CRC_MDS_crc32_b0 | GPL_CR1_CRC_EN_enable_b0;
  //RB(GPL_CR1_b0)= GPL_CR1_CRC_DSIZE_32bit_b0 | GPL_CR1_CRC_MDS_crc32_b0 | GPL_CR1_CRC_EN_enable_b0;
}
*/

//uint32_t crc32_calc(uint8_t val) {
//  RB(GPL_DIN_b0) = val;
//  return RW(GPL_DIN_w);
//}

uint32_t crc32_block8(const void* buf, uint32_t len) {
  uint32_t i;
  RB(GPL_CR1_b0)=0; // сброс модуля - выполнять обязательно!
  RB(GPL_CR0_b0)=GPL_CR0_BREV_MDS_8bit_b0; // перестановка битов в байте
  RW(GPL_CRCINIT_w)=0xffffffff; // начальное значение
  RB(GPL_CR1_b0)=
    GPL_CR1_CRC_BREV_32bit_b0 | // перестановка битов в 32-битной сумме
    GPL_CR1_CRC_DSIZE_8bit_b0 | // размер входных данных - 1 байт
    GPL_CR1_CRC_MDS_crc32_b0 |  // режим CRC32
    GPL_CR1_CRC_EN_enable_b0;   // включаем модуль
  for (i=0; i<len; i++) {
    RB(GPL_DIN_b0) = ((uint8_t*)buf)[i];
  }
  return RW(GPL_DOUT_w) ^ 0xffffffff; // по стандарту Ethernet нужно выполнить инверсию
}


uint32_t crc32_block8_hw(const void* buf, uint32_t len) {
  uint32_t i;
  RB(GPL_CR1_b0)=0;
  RW(GPL_CRCINIT_w)=0xffffffff;
  RW(GPL_DIN_w)=0;
  RB(GPL_CR1_b0)= GPL_CR1_CRC_DSIZE_8bit_b0 | GPL_CR1_CRC_MDS_crc32_b0 | GPL_CR1_CRC_EN_enable_b0;
  for (i=0; i<len; i++) {
    RB(GPL_DIN_b0) = ((uint8_t*)buf)[i];
    //__NOP();
    //__NOP();
  }
  return RW(GPL_DOUT_w);
}


uint32_t crc32_block32_hw(const uint32_t* buf, uint32_t len) {
  uint32_t i;
  //RB(GPL_CR0_b0)= GPL_CR0_BREV_MDS_32bit_b0 | GPL_CR0_BEND_EN_enable_b0;
  RB(GPL_CR0_b0)= GPL_CR0_BEND_EN_enable_b0;
  RB(GPL_CR1_b0)=0;
  RW(GPL_CRCINIT_w)=0xffffffff;
  RW(GPL_DIN_w)=0;
  //RB(GPL_CR1_b0)= GPL_CR1_CRC_DSIZE_32bit_b0 | GPL_CR1_CRC_MDS_crc32_b0 | GPL_CR1_CRC_EN_enable_b0;
  RB(GPL_CR1_b0)= GPL_CR1_CRC_DSIZE_32bit_b0 | GPL_CR1_CRC_MDS_crc32_b0 | GPL_CR1_CRC_EN_enable_b0;
  len >>= 2;
  for (i=0; i<len; i++) {
    RW(GPL_DIN_w) = buf[i];
    __NOP(); __NOP();// __NOP(); __NOP();
  }
  return RW(GPL_DOUT_w);
}


uint32_t crc32_block32_sw_le(const uint32_t* buf, uint32_t len) {
  uint32_t i=0, j=0, c=0xFFFFFFFF;
  len >>= 2;
  for (i=0; i<len; i++) {
    c ^= buf[i];
    for (j=0; j<32; j++)
      c = c & 0x80000000 ? (c << 1) ^ 0x04C11DB7 : c << 1;
  }
  return c;
}


uint32_t crc32_block32_sw_be(const uint32_t* buf, uint32_t len) {
  uint32_t i=0, j=0, c=0xFFFFFFFF;
  len >>= 2;
  for (i=0; i<len; i++) {
    c ^= __REV(buf[i]);
    for (j=0; j<32; j++)
      c = c & 0x80000000 ? (c << 1) ^ 0x04C11DB7 : c << 1;
  }
  return c  ^ 0xFFFFFFFF;
}


// https://www.csie.ntu.edu.tw/~cyy/courses/introCS/13fall/assignments/proj4/index.html
uint32_t crc32_block32_sw2(const uint8_t* buf, uint32_t len) {
  uint32_t i=0, j=0, c=0xFFFFFFFF;
  for (i=0; i<len; i++) {
    c ^= (uint32_t)buf[i];
    for (j=0; j<8; j++)
      c = c & 0x01 ? (c >> 1) ^ 0xEDB88320 : c >> 1;
  }
  return c ^ 0xFFFFFFFF;
}
