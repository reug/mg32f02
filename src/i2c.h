#ifndef MG32F02_I2C_H
#define MG32F02_I2C_H

#include "MG32x02z__RegAddress.h"
#include <stdint.h>

#define I2C0_id I2C0_Base
#define I2C1_id I2C1_Base


/// Инициализация модуля I2C (включение тактирования)
void i2c_init(uint32_t id);

/// Установка режима работы модуля I2C по формату регистра CR0 (устанавливает I2Cx_EN)
void i2c_setup_mode(uint32_t id, uint32_t mode);

/// Настройка тактирования согласно формату регистра CLK
void i2c_setup_clock(uint32_t id, uint16_t mode);

/// Включение прерывания INT_I2Cx по флагам, указанным в flags согласно формату I2Cx_INT
void i2c_setup_int(uint32_t id, uint32_t flags);


void i2c_master_start(uint32_t id);

void i2c_master_stop(uint32_t id);

/// Режим master: передача в режиме Buffer байта из data.
void i2c_master_send(uint32_t id, uint8_t data);


#endif // MG32F02_I2C_H
