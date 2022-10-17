#ifndef MG32F02_I2C_H
#define MG32F02_I2C_H

#include "MG32x02z__RegAddress.h"
#include "MG32x02z_I2C.h"
#include "core.h"
#include <stdint.h>

#define I2C0_id I2C0_Base
#define I2C1_id I2C1_Base

/// Опции по формату регистра I2Cx_CR2
enum I2C_Options {
  I2C_NOOPTS    = 0,
  I2C_NACK      = I2C_CR2_CMD_TC_enable_w | I2C_CR2_AA_LCK_un_locked_w,
  I2C_ACK       = I2C_CR2_CMD_TC_enable_w | I2C_CR2_AA_LCK_un_locked_w | I2C_CR2_PAA_mask_w,
  I2C_STOP      = I2C_CR2_CMD_TC_enable_w | I2C_CR2_STO_LCK_un_locked_w | I2C_CR2_PSTO_mask_w,
  I2C_START     = I2C_CR2_CMD_TC_enable_w | I2C_CR2_STA_LCK_un_locked_w | I2C_CR2_PSTA_mask_w
};


/// Инициализация модуля I2C (включение тактирования)
void i2c_init(uint32_t id);

/// Установка режима работы модуля I2C по формату регистра CR0 (устанавливает I2Cx_EN)
inline
void i2c_setup_mode(uint32_t id, uint32_t mode) {
  RW(id+( I2C0_CR0_w -I2C0_Base)) = mode | I2C_CR0_EN_enable_w; // включаем модуль
}
/*
/// Настройка тактирования согласно формату регистра CLK
void i2c_setup_clock(uint32_t id, uint16_t mode);
*/
/// Включение прерывания INT_I2Cx по флагам, указанным в flags согласно формату I2Cx_INT
void i2c_setup_int(uint32_t id, uint32_t flags);

/// Перезапуск модуля с текущими настройками
void i2c_reset(uint32_t id);

/// Генерирует состояние START + WRITE с указанным адресом (младший бит должен быть 0).
void i2c_master_startw(uint32_t id, uint8_t addr);

/// Генерирует состояние START + READ с указанным адресом (младший бит должен быть 0).
void i2c_master_startr(uint32_t id, uint8_t addr);

/// Генерирует состояние STOP.
void i2c_master_stop(uint32_t id);

/// Ожидает состояние REPEAT START.
void i2c_wait_start(uint32_t id);

/// Ожидает состояние STOP.
void i2c_wait_stop(uint32_t id);

/// Режим master: передача в режиме Buffer len байт (1-4) из data.
/// Генерирует состояние STOP, если указана опция I2C_STOP.
/// Блокирующая функция с таймаутом: ожидает флаг TXF.
void i2c_master_send(uint32_t id, uint32_t opts, uint8_t len, uint32_t data);

/// Режим master: прием в режиме Buffer len байт (1-4).
/// Генерирует ACK, если указана опция I2C_ACK.
/// Генерирует состояние STOP, если указана опция I2C_STOP.
/// Блокирующая функция с таймаутом: ожидает флаг RXF.
uint32_t i2c_master_recv(uint32_t id, uint32_t opts, uint8_t len);


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

#endif // MG32F02_I2C_H
