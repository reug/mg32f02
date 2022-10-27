#ifndef SPI_TEST_H
#define SPI_TEST_H

#include "spi.h"

void spi_test_master();
void spi_test_slave();

/// Настройка внешнего прерывания EXIC
void exint_setup();

/// Настройки DMA
void spi_dma_setup();

/// Запуск процедуры приема данных на канале 0, n - число байт
void spi_dma_start_rx(uint32_t n);

/// Запуск процедуры отправки данных на канале 1, n - число байт
void spi_dma_start_tx(uint32_t n);

/// Возвращает IPv4 адрес из входящего пакета ARP запроса.
/// Если пакет не содержит ARP-запрос, возвращает 0.
uint32_t arp_get_ipaddr(const uint8_t* pkt);

/// Формирует пакет с ARP-запросом.
void arp_gen_reply(uint8_t* pkt);

#endif
