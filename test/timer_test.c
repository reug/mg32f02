#include "timer_test.h"
#include "timer.h"
#include "core.h"
#include "ulib.h"
#include "utils.h"
#include "MG32x02z_TM.h"

/*
p. 393:
## CKO output from Main Timer

User can select the timer clock output coming from Main Timer by setting TMx_CKO_SEL register. When
the timer operation mode is set Separate Mode, use TMx_CK_TC as timer input clock.
The time period is set by Main Timer auto-reload register of TMx_ARR.
When the timer operation mode is set Full-Counter Mode or Cascade Mode,
use TMx_CK_TC2 as timer input clock. The time period is set by both TMx_ARR and TMx_PSARR registers.
*/


// Обработчик прерывания
void timer_hdl() {
  RH(PC_SC_h0) = 2; // set PC1
  __NOP(); __NOP();
  RH(PC_SC_h1) = 2; // clear PC1
  RB(TM00_STA_b0) = 0xFF; // clear flags
}


void tm00_setup_separate() {
  // Настройка режима работы
  RH(TM00_CR0_h0) =
    TM_CR0_DIR2_down_h0 |     // CT2 считает вниз
    TM_CR0_MDS_separate_h0 |  // включаем раздельный режим
    TM_CR0_EN2_enable_h0 |
    TM_CR0_EN_enable_h0;

  // K1 = 150, K2 = 150
  RB(TM00_ARR_b0) = 150-1;
  RB(TM00_PSARR_b0) = 100-1;
}


void tm00_setup_cascade() {
  // Настройка режима работы
  RH(TM00_CR0_h0) =
    TM_CR0_MDS_cascade_h0 |  // включаем каскадный режим
    TM_CR0_EN2_enable_h0 |
    TM_CR0_EN_enable_h0;

  // Kmain = 15, K2 = 100, Fout = 1 kHz:
  RB(TM00_ARR_b0) = 15-1;
  RB(TM00_PSARR_b0) = 100-1;
}


void tm00_setup_fullcnt() {
  // Настройка режима работы
  RH(TM00_CR0_h0) =
    TM_CR0_MDS_full_counter_h0 |    // включаем 16-битный режим
    //TM_CR0_EN2_enable_h0 | // необязательно для Full-Counter mode
    TM_CR0_EN_enable_h0;

  // K = 1500 (0x5DC), Fout = 1 kHz:
  RB(TM00_ARR_b0) = 0x05;
  RB(TM00_PSARR_b0) = 0xDC-1;
}


void tm00_test() {

// Прерывание по TOF пока не срабатывает
/*
  // Настройка прерывания
  RH(PC_CR1_h0) = 0x0002; // Выход таймера -> PC1
  RH(PC_SC_h1) = 2; // clear PC1
  SVC2(SVC_HANDLER_SET,12,timer_hdl); // устанавливаем обработчик прерывания IRQ#12
  tm00_set_int(TM_INT_TIE_enable_b0 | TM_INT_IEA_enable_b0);
*/

  tm00_init();
  // Настройка тактирования: TM00_CK_TC2 - 1500000 Гц
  RH(TM00_CLK_h0) =
    TM_CLK_CKI_DIV_div8_h0 | // TM00_CKI_DIV: DIV8
    TM_CLK_CKI_SEL_proc_h0 | // TM00_CKI_SEL: CK_TMx_PR
    TM_CLK_CKS2_SEL_ck_int_h0 ;// TM00_CKS2_SEL: CK_INT

  //tm00_setup_separate();
  //tm00_setup_cascade();
  tm00_setup_fullcnt();


  // Настройка триггеров
  RH(PD_CR9_h0) = (0x2 << 12) | 2; // PD9: TM00_TRGO, push pull output
  RW(TM00_TRG_w) =
  //TM00_TRGO_MDS:
    //TM_TRG_TRGO_MDS_tof_w; // 0x3 = TOF : TM00_TOF (Main Timer overflow)
    //TM_TRG_TRGO_MDS_tof2_w;
    TM_TRG_TRGO_MDS_uev_w; // 0x2 = UEV : TM00_UEV (Main Timer Update event)
    //TM_TRG_TRGO_MDS_uev2_w; // UEV2 (Prescaler Update event)


  // Настройка выхода
  RH(PB_CR0_h0) = (0x4 << 12) | 2; // TM00_CKO, push pull output
  RB(TM00_CKO_b0) =
    //TM_CKO_CKO_SEL_2nd_b0 |
    TM_CKO_CKO_SEL_main_b0 |
    TM_CKO_CKO_EN_enable_b0;
  /*
  Выход CKO работает как T-триггер - делит частоту события на 2 и выдает всегда меандр!!!
  Еще вопрос, а когда вообще должен переключаться выход CKO ?
  В описании регистров написано, что это overflow (?):
    Timer overflow as CKO output source select.
    0 = 2nd : 2nd Timer overflow
    1 = Main : Main Timer overflow

  Для tm00_setup_cascade():
  Если выводить сигнал через TRGO, то частота будет правильная, а импульсы короткие.
  Но выбирать надо событие UEV! Событие TOF не работает.
  */

}
