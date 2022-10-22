#ifndef MG32F02_SPI_H
#define MG32F02_SPI_H

#include "MG32x02z_SPI.h"
#include "core.h"
#include <stdint.h>


/// Опции конфигурации по формату регистра SPI0_CR0
enum SPI_Config {
  SPI_CPOL_LOW    = SPI_CR0_CPOL_low_w,             // CPOL=0
  SPI_CPOL_HIGH   = SPI_CR0_CPOL_high_w,            // CPOL=1
  SPI_CPHA_LE     = SPI_CR0_CPHA_leading_edge_w,    // CPHA=0
  SPI_CPHA_TE     = SPI_CR0_CPHA_trailing_edge_w,   // CPHA=1
  SPI_MSB         = SPI_CR0_LSB_EN_disable_w,
  SPI_LSB         = SPI_CR0_LSB_EN_enable_w,
  SPI_SLAVE       = SPI_CR0_MDS_slave_w,
  SPI_MASTER      = SPI_CR0_MDS_master_w,
  SPI_NSSO_EN     = SPI_CR0_NSSO_EN_enable_w,       // Разрешить управление сигналом NSSO
  SPI_NSSI_EN     = SPI_CR0_NSSI_EN_enable_w,       // Разрешить управление сигналом NSSI
  SPI_NSS_SWEN    = SPI_CR0_NSS_SWEN_enable_w,      // Разрешить программное управление сигналом NSS
  SPI_NSSO_INV    = SPI_CR0_NSSO_INV_enable_w,      // Инвертировать NSSO
  SPI_NSS_PEN     = SPI_CR0_NSS_PEN_enable_w        // Импульс сигнала NSS между кадрами
};


///// Опции по формату регистра SPI0_CR2
//enum SPI_Options2 {
//  SPI_SWO
//}


/// Инициализация модуля SPI0 (включение тактирования)
void spi_init();

/// Установка режима работы модуля SPI0 по формату регистра CR0 (устанавливает SPI0_EN)
inline
void spi_setup_mode(uint32_t mode) {
  RW(SPI0_CR0_w) = mode | SPI_CR0_EN_enable_w; // включаем модуль
}

/// Включение прерывания INT_SPI0 по флагам, указанным в flags согласно формату SPI0_INT
void spi_setup_int(uint16_t flags);

/// Управление состоянием NSS (chip select)
inline
void spi_nss(uint8_t state) {
  RB(SPI0_CR2_b3) = (state & SPI_CR2_NSS_SWO_mask_b3);
  //RB(SPI0_CR2_b3) |= (state & SPI_CR2_NSS_SWO_mask_b3);
}

/// Установка длительности импульса сигнала NSS (0 - 1T, 1 - 2T)
inline
void spi_nss_pulse(uint8_t v) {
  RB(SPI0_CR1_b2) = v;
}

/// RX buffer flush. Сбрасывает RXF и RX_LVL.
inline
void spi_flush_rx() {
  RB(SPI0_CR1_b0) = SPI_CR1_RDAT_CLR_enable_b0; // Flush RX buffer
}

/// TX buffer flush. Сбрасывает TX_LVL.
inline
void spi_flush_tx() {
  RB(SPI0_CR1_b0) = SPI_CR1_TDAT_CLR_enable_b0; // Flush RX buffer
}

/// Передача одного байта.
/// Размер кадра должен быть настроен на 8 бит.
/// Возвращает прочитанный в процессе передачи байт.
uint8_t spi_tx(uint8_t data);

/// Прием одного байта.
/// Размер кадра должен быть настроен на 8 бит.
inline
uint8_t spi_rx() {return spi_tx(0xFF);}

/// Передача (1-4) из data.
/// Блокирующая функция с таймаутом: ожидает флаг TXF.
void spi_send(uint32_t opts, uint8_t len, uint32_t data);

/// Прием данных.
/// Блокирующая функция с таймаутом: ожидает флаг RXF.
uint32_t spi_recv(uint32_t opts, uint8_t len);

/*
/// Запись данных в буфер отправки (1-4 байта)
inline
void i2c_write(uint32_t id, uint32_t data, uint8_t len) {
  RB(id+( I2C0_CR2_b1 -I2C0_Base)) = (len & I2C_CR2_BUF_CNT_mask_b1); // BUF_CNT
  RW(id+( I2C0_DAT_w -I2C0_Base)) = data;
}

/// Чтение буфера приема (4 байта)
inline
uint32_t i2c_read(uint32_t id) {
  return RW(id+( I2C0_DAT_w -I2C0_Base));
}


/// Запись данных из буфера программы в буфер отправки.
/// На входе: *p - текущее положение в буфере (указывает на следующий байт после отправленного).
/// На выходе: *p - новое значение, увеличенное на число отправленных байт.
/// Возвращает оставшееся число байт.
uint32_t i2c_writebuf(uint32_t id, const void* buf, uint32_t* p, uint32_t len);


/// Чтение принятых данных и их запись в указанный буфер.
/// На входе: *p - текущее положение в буфере (указывает на следующий байт после записанного).
/// На выходе: *p - новое значение, увеличенное на число считанных байт.
/// Возвращает фактическое число считанных байт.
uint8_t i2c_readbuf(uint32_t id, void* buf, uint32_t* p);


/// Настройка таймера таймаута, режим работы mode определяется по формату младшего байта регистра I2Cx_TMOUT.
inline
void i2c_setup_tmout(uint32_t id, uint8_t mode) {
  RH(id+( I2C0_TMOUT_h0 -I2C0_Base)) =
    (38 << I2C_TMOUT_TMO_CNT_shift_h0) | // период счета ~1 мс для F(CK_TMO)=37.5 кГц
    mode |
    // I2C_TMOUT_TMO_MDS_scl_low_h0 |
    // I2C_TMOUT_TMO_MDS_scl_sda_high_h0 |
    I2C_TMOUT_TMO_CTL_enable_h0 |
    I2C_TMOUT_TMO_EN_enable_h0;
}


/// Возвращает I2Cx_STA
inline
uint32_t i2c_get_status(uint32_t id) {
  return RW(id+( I2C0_STA_w -I2C0_Base));
}

/// Возвращает 1, если таймаут, иначе 0.
inline
uint32_t i2c_get_tmout(uint32_t id) {
  return (RB(id+( I2C0_STA_b0 -I2C0_Base)) & I2C_STA_TMOUTF_happened_b0) != 0;
}

/// Сбрасывает флаг(и) статуса mask по формату регистра I2Cx_STA
inline
void i2c_clr_status(uint32_t id, uint32_t mask) {
  RW(id+( I2C0_STA_w -I2C0_Base)) = mask;
}

#ifdef I2C_DEBUG

extern uint32_t i2c_status[16]; ///< Массив статусов I2C для дальнейшего анализа

/// Выводит текстовой строки статуса в терминал в порядке убывания номера флага (3 строки по 48 символов)
void i2c_print_status(uint32_t status);
#endif
*/

#endif // MG32F02_SPI_H
