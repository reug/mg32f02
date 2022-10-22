#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>

/// Максимальный размер буфера для кадров Ethernet
#define ETH_FRAME_MAXSIZE 1520

/// Глобальный буфер для кадров Ethernet
extern
volatile uint8_t eth_frame[ETH_FRAME_MAXSIZE];


/* Функции для работы с буфером */

/// Fills Ethernet frame with local address
void eth_setframe_addr(const uint8_t* addr);

/// Fills Ethernet frame with target address
void eth_setframe_dest(const uint8_t* dest);

/// /// Fills Ethernet frame with length
void eth_setframe_size(uint16_t len);

/// Swap Ethernet SAADR and DADDR in 'eth_frame'
void eth_setframe_swap();


/* Функции, зависящие от адаптера */

/// Inits ENC28J60 chip.
void eth_init(const uint8_t *macadr);

/// Включение прерывания
void eth_setup_int();

/// Сброс флагов прерывания
void eth_clear_int();

/// Sends Ethernet packet.
void eth_sendpkt(uint8_t *data, uint16_t len);

/// Receives Ethernet packet. Returns received frame size, 0 if nothing is received.
uint16_t eth_recvpkt(uint8_t *buf, uint16_t buflen);

/// Возвращает локальный MAC-адрес, установленный в адаптере
void eth_get_addr(uint8_t* addr);

#endif //  ETHERNET_H
