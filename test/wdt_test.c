#include "wdt_test.h"
#include "wdt.h"
#include "MG32x02z_IWDT.h"
#include "MG32x02z_WWDT.h"
#include "MG32x02z_RST.h"
#include "core.h"
#include "ulib.h"
#include "utils.h"


// Обработчик прерывания IWDT (INT_SYS)
void iwdt_hdl() {
  uart_puts(PORT,"INT:IWDT",UART_NEWLINE_CRLF);
  RB(IWDT_STA_b0) = 0xFF; // Сбрасываем флаги IWDT_STA
}


// Тест сторожевого таймера IWDT
void iwdt_test() {
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
  }
}


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
