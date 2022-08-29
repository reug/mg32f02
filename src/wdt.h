#ifndef WDT_H
#define WDT_H

#include <stdint.h>

/// Инициализация IWDT
void iwdt_init();

/// Перезагрузка IWDT
void iwdt_reload();

/// Разблокировка записи регистров IWDT
void iwdt_write_unlock();

/// Блокировка записи регистров IWDT
void iwdt_write_lock();

/// Включение прерывания INT_SYS по флагам, указанным в flags согласно формату IWDT_INT
void iwdt_set_int(uint8_t flags);

/// Инициализация WWDT
void wwdt_init();

/// Перезагрузка WWDT
void wwdt_reload();

/// Разблокировка записи регистров WWDT
void wwdt_write_unlock();

/// Блокировка записи регистров WWDT
void wwdt_write_lock();

/// Включение прерывания INT_WWDT по флагам, указанным в flags согласно формату WWDT_INT
void wwdt_set_int(uint8_t flags);

#endif // WDT_H
