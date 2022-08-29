#include "wdt_test.h"
#include "wdt.h"
#include "MG32x02z_IWDT.h"
#include "MG32x02z_WWDT.h"
#include "MG32x02z_RST.h"
#include "core.h"
#include "ulib.h"
#include "utils.h"


void debug_reset_status() {
  uint32_t d;
  d = RW(RST_STA_w);
  debug32('F',d);
  if (d & RST_STA_CRF_mask_w) uart_puts(PORT,"RST:COLD",UART_NEWLINE_CRLF);
  if (d & RST_STA_WRF_mask_w) uart_puts(PORT,"RST:WARM",UART_NEWLINE_CRLF);
  if (d & RST_STA_WWDTF_mask_w) uart_puts(PORT,"SRC:WWDT",UART_NEWLINE_CRLF);
  if (d & RST_STA_IWDTF_mask_w) uart_puts(PORT,"SRC:IWDT",UART_NEWLINE_CRLF);
  // Сбрасываем все флаги:
  RW(RST_STA_w) = 0xFFFFFFFF;
}


// Обработчик прерывания IWDT (INT_SYS)
void iwdt_hdl() {
  uart_puts(PORT,"INT:IWDT",UART_NEWLINE_CRLF);
  RB(IWDT_STA_b0) = 0xFF; // Сбрасываем флаги IWDT_STA
}


// Тест сторожевого таймера IWDT
void iwdt_test() {
  uint32_t i=0;
  SVC2(SVC_HANDLER_SET,1,iwdt_hdl); // устанавливаем обработчик прерывания INT_SYS
  iwdt_set_int(IWDT_INT_EW0_IE_enable_b0); // разрешаем прерывание IWDT_EW0_IE

  iwdt_init(); // включаем общее тактирование IWDT в CSC
  iwdt_write_unlock();
  RB(IWDT_CLK_b0) = IWDT_CLK_CK_DIV_div1024_b0; // выбираем делитель частоты 4096
  RB(IWDT_CR0_b0) = IWDT_CR0_EN_enable_b0; // включаем IWDT_EN=1
  iwdt_write_lock();

  // Разрешаем горячий сброс:
  RH(RST_KEY_h0) = 0xA217; // разблокируем запись в регистры RST
  RW(RST_WE_w) |= RST_WE_IWDT_WE_enable_w;
  RH(RST_KEY_h0) = 0; // возвращаем блокировку записи в регистры RST

  // Выводим в терминал значение счетчика:
  while (1) {
    __disable_irq();
    debug('C',RB(IWDT_CNT_b0));
    __enable_irq();
    delay_ms(50);
//    if (++i==100) {
//      iwdt_reload();
//      uart_puts(PORT,"RELOAD",UART_NEWLINE_CRLF);
//      i=0;
//    }
  }
}


void wwdt_hdl() {
  uart_puts(PORT,"INT:WRN",UART_NEWLINE_CRLF);
  RB(WWDT_STA_b0) = 0xFF; // Сбрасываем флаги
}


// Тест сторожевого таймера WWDT
void wwdt_test() {
  uint32_t i=0;
  SVC2(SVC_HANDLER_SET,0,wwdt_hdl); // устанавливаем обработчик прерывания
  wwdt_set_int(WWDT_INT_WRN_IE_enable_b0); // разрешаем прерывание WWDT_WRN_IE

  wwdt_init(); // включаем общее тактирование WWDT в CSC
  wwdt_write_unlock();
  RB(WWDT_CLK_h0) =
    WWDT_CLK_CK_PDIV_divided_by_256_h0 |  // делитель частоты PDIV: 256
    WWDT_CLK_CK_DIV_div128_h0;            // делитель частоты DIV: 128
  RB(WWDT_CR0_b0) =
    WWDT_CR0_RSTW_EN_enable_b0 | // разрешаем сброс по выходу из окна
    WWDT_CR0_RSTF_EN_enable_b0 | // разрешаем сброс по завершению счета
    WWDT_CR0_EN_enable_b0;       // включаем IWDT_EN=1
  RH(WWDT_WIN_h0) = 700;         // устанавливаем окно
  RH(WWDT_WRN_h0) = 690;         // устанавливаем время предупреждения
  wwdt_write_lock();

  // Разрешаем горячий сброс от модуля WWDT:
  RH(RST_KEY_h0) = 0xA217; // разблокируем запись в регистры RST
  RW(RST_WE_w) |= RST_WE_WWDT_WE_enable_w;
  RH(RST_KEY_h0) = 0; // возвращаем блокировку записи в регистры RST

  // Выводим в терминал значение счетчика:
  while (1) {
    __disable_irq();
    debug('C',RH(WWDT_CNT_h0));
    __enable_irq();
    delay_ms(50);
    if (++i==20) {
      wwdt_reload();
      uart_puts(PORT,"RELOAD",UART_NEWLINE_CRLF);
      i=0;
    }
  }
}


// Выключение "горячего" сброса самого таймера WWDT.
// TODO: разработать методику испытания. Пока не срабатывает как описано.
void rst_wwdt_wdis() {
  RH(RST_KEY_h0) = 0xA217; // разблокируем запись в регистры RST
  RW(RST_CR0_b3) |= RST_CR0_WWDT_WDIS_disable_b3;
  RH(RST_KEY_h0) = 0; // возвращаем блокировку записи в регистры RST
}
