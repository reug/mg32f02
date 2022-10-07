#ifndef MG32F02_I2C_H
#define MG32F02_I2C_H

#include "MG32x02z__RegAddress.h"
#include "MG32x02z_I2C.h"
#include "core.h"
#include <stdint.h>

#define I2C0_id I2C0_Base
#define I2C1_id I2C1_Base

enum I2C_Options {
  I2C_ACK       = 0x40,
  I2C_STOP      = 0x20,
  I2C_START     = 0x10
};


/// Инициализация модуля I2C (включение тактирования)
void i2c_init(uint32_t id);

/// Установка режима работы модуля I2C по формату регистра CR0 (устанавливает I2Cx_EN)
void i2c_setup_mode(uint32_t id, uint32_t mode);

/// Настройка тактирования согласно формату регистра CLK
void i2c_setup_clock(uint32_t id, uint16_t mode);

/// Включение прерывания INT_I2Cx по флагам, указанным в flags согласно формату I2Cx_INT
void i2c_setup_int(uint32_t id, uint32_t flags);

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
/// Генерирует состояние STOP, если бит 7 len установлен.
void i2c_master_send(uint32_t id, uint8_t len, uint32_t data);

/// Режим master: прием в режиме Buffer len байт (1-4).
/// Генерирует ACK, если бит 6 len установлен.
/// Генерирует состояние STOP, если бит 7 len установлен.
/// Блокирующая функция: ожидает флаг RXF.
uint32_t i2c_master_recv(uint32_t id, uint8_t len);

/// Возвращает I2Cx_STA
inline
uint32_t i2c_get_status(uint32_t id) {return RW(id+( I2C0_STA_w -I2C0_Base));}

#ifdef I2C_DEBUG

extern uint32_t i2c_status[16]; ///< Массив статусов I2C для дальнейшего анализа

/// Выводит текстовой строки статуса в терминал в порядке убывания номера флага (3 строки по 48 символов)
void i2c_print_status(uint32_t status);
#endif

#endif // MG32F02_I2C_H
