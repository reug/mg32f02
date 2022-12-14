#include "st_rtc_test.h"
#include "rtc.h"
#include "core.h"
#include "MG32x02z_RTC.h"
#include "utils.h"


// Чтение значения CURRENT после сброса
void systick_test_read_current() {
  volatile register int32_t t0;
  char s[12];
  RW(CPU_SYST_CSR_w) = 0; // Stop timer
  RW(CPU_SYST_RVR_w) = 15000; //(1 << 24) -1; // RELOAD
  RW(CPU_SYST_CVR_w) = 1; // Clear CURRENT
  //*(volatile uint32_t*)CPU_SYST_CSR_w = 1; // CLKSOURCE = 0 (0-CK_ST, 1-CPU), ENCNT=1
  t0 = RW(CPU_SYST_CVR_w);
  strUint32hex(s,t0);
  uart_puts(PORT,s,UART_NEWLINE_CRLF);
  while (1);
}


void systick_test1() {
  volatile register int32_t t0,t1;
  char s[12];
  RW(CPU_SYST_CSR_w) = 0; // Stop timer
  RW(CPU_SYST_RVR_w) = 15000; //(1 << 24) -1; // RELOAD
  RW(CPU_SYST_CVR_w) = 1; // Clear CURRENT
  RW(CPU_SYST_CSR_w) = 1; // CLKSOURCE = 0 (0-CK_ST, 1-CPU), ENCNT=1
  while (1) {
    RW(CPU_SYST_CVR_w) = 1; // Clear CURRENT
    t0 = RW(CPU_SYST_CVR_w);
    delay_ms(1);
    t1 = RW(CPU_SYST_CVR_w);
    strUint32hex(s,t0-t1);
    uart_puts(PORT,s,UART_NEWLINE_CRLF);
    delay_ms(500);
  }
}

// Обработчик исключения SysTick
void systick_hdl() {
  RH(PC_SC_h0) = 2; // set PC1
  RH(PC_SC_h1) = 2; // clear PC1
}

// Функция тестирования SysTick Timer
void systick_test() {
  RH(PC_CR1_h0) = 0x0002; // Выход таймера -> PC1
  SVC2(SVC_CHANDLER_SET,15,systick_hdl);   // Устанавливаем обработчик исключения 15
  RW(CPU_SYST_CSR_w) = 0; // Stop timer
  RW(CPU_SYST_RVR_w) = 1500-1; // RELOAD
  RW(CPU_SYST_CVR_w) = 1; // Clear CURRENT
  RW(CPU_SYST_CSR_w) = 3; // CLKSOURCE = 0 (External), TICKINT=1, ENCNT=1
  while (1) ;
}


// Вывод счетных импульсов на RTC_OUT
void rtc_test_clock() {
  // RTC_OUT setup:
  RH(PD_CR10_h0) = (5 << 12) | 2; // PD10: RTC_OUT, push-pull output
  //RH(PD_CR10_h0) = 2;  RH(PD_SC_h0) = (1 << 10); // PD10: push-pull output test
  // Альтернативный вариант:
  // RH(PB_CR8_h0) = (2 << 12) | 2; // PB8 -> RTC_OUT, push pull output

  rtc_init();
  rtc_write_unlock();
  // Вариант на 1000 Гц от CK_UT 4 кГц
//  csc_set_ck_ut();
//  RB(RTC_CLK_b0) =
//      RTC_CLK_CK_PDIV_div1_b0 |
//      RTC_CLK_CK_DIV_div4_b0 |
//      RTC_CLK_CK_SEL_ck_ut_b0;   // Используем сигнал CK_UT (Unit clock) для тактирования RTC
  // Вариант на 1500 кГц от CK_APB 12 МГц
  RB(RTC_CLK_b0) =
      RTC_CLK_CK_PDIV_div1_b0 |
      RTC_CLK_CK_DIV_div8_b0 |
      RTC_CLK_CK_SEL_ck_apb_b0;
  RB(RTC_CR0_b0) =
      RTC_CR0_EN_enable_b0;// RTC_EN = 1
  rtc_write_lock();
  rtc_set_out(RTC_CR0_OUT_SEL_pc_b1); // PC (CK_RTC_INT)
}


#define RTC_ALARM_ADD 15000 // 10 мс


// Обработчик прерывания RTC
void rtc_hdl() {
  RH(PB_SC_h0) = (1 << 13); // Тестовый сигнал прерывания PB13 HI
  uint32_t d;
  d=RW(RTC_ALM_w);
  rtc_write_unlock();
  if (d == 1000*RTC_ALARM_ADD) {
    d=0;
    RW(RTC_RLR_w)=0;
    RB(RTC_CR1_b0) = 1; // RTC_RC_START
  }
  // Обновляем значение ALARM:
  RB(RTC_CR0_b0) &= ~RTC_CR0_ALM_EN_enable_b0; // RTC_ALM_EN=0
  RW(RTC_ALM_w) = d + RTC_ALARM_ADD;
  RB(RTC_CR0_b0) |= RTC_CR0_ALM_EN_enable_b0; // RTC_ALM_EN=1

  rtc_write_lock();
  RB(RTC_STA_b0) = RTC_STA_ALMF_mask_b0; // Clear ALMF flag
  RH(PB_SC_h1) = (1 << 13); // Тестовый сигнал прерывания PB13 LO
}


// Тестирование режима ALARM с прерыванием
void rtc_test_alarm() {
  // RTC_OUT setup:
  RH(PD_CR10_h0) = (5 << 12) | 2; // PD10: RTC_OUT, push-pull output
  //RH(PD_CR10_h0) = 2;  RH(PD_SC_h0) = (1 << 10); // PD10: push-pull output test
  // Альтернативный вариант:
  // RH(PB_CR8_h0) = (2 << 12) | 2; // PB8 -> RTC_OUT, push pull output

  rtc_init();
  rtc_write_unlock();
  // Вариант на 1500 кГц от CK_APB 12 МГц
  RB(RTC_CLK_b0) =
      RTC_CLK_CK_PDIV_div1_b0 |
      RTC_CLK_CK_DIV_div8_b0 |
      RTC_CLK_CK_SEL_ck_apb_b0;
  RW(RTC_ALM_w) = RTC_ALARM_ADD; // Alarm через 10 мс
  RW(RTC_CR0_w) =
      //RTC_CR0_OUT_LCK_un_locked_w | RTC_CR0_OUT_STA_1_w |    // RTC_OUT_LCK = 1 & RTC_OUT_STA = 1
      RTC_CR0_RCR_MDS_forced_reload_w |  // RTC_RCR_MDS = 2 (Force Reload)
      RTC_CR0_ALM_EN_enable_w |          // Включаем режим ALARM
      RTC_CR0_EN_enable_w;               // RTC_EN = 1
  rtc_write_lock();

  SVC2(SVC_HANDLER_SET,1,rtc_hdl); // Устанавливаем обработчик прерывания
  rtc_set_int(RTC_INT_ALM_IE_enable_b0); // Разрешаем прерывание по флагу ALMF
  rtc_set_out(RTC_CR0_OUT_SEL_alm_b1); // Настраиваем выход RTC_OUT Alarm
}
