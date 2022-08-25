#ifndef INIT_H
#define INIT_H

#include <stdint.h>

/// CSC init
void init_clock();

/// CK_ICKO output through PC0 pin
void setup_icko();

/// Установка частоты IHRCO на 11,0592 МГц
void setup_ihrco();

/// Включение XOSC генератора (CK_MAIN, DIV=4).
/// Возвращает 1 в случае успеха, иначе 0.
uint8_t setup_xosc();

/// Включение умножения частоты на основе PLL (x2)
void setup_pll();

/// Включение сигнала CK_UT на 4 кГц
void csc_set_ck_ut();

/// Включение XOSC без выбора его в качестве источника
void csc_xosc_init();

/// Выбор XOSC в качестве источника CK_HS
void csc_xosc_select();

#endif
