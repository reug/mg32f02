#include "timer_test.h"
#include "timer.h"
#include "core.h"
#include "ulib.h"
#include "utils.h"
#include "init.h"
#include "MG32x02z_TM.h"
#include "MG32x02z_APB.h"

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


void tm00_setup_separate(uint16_t khi, uint16_t klo) {
  // Настройка режима работы
  RH(TM00_CR0_h0) =
    TM_CR0_DIR2_up_h0 |     // CT2 считает вверх
    TM_CR0_MDS_separate_h0 |  // включаем раздельный режим
    TM_CR0_EN2_enable_h0 |    // включаем CT2
    TM_CR0_EN_enable_h0;      // включаем CT1

  RB(TM00_ARR_b0) = khi;
  RB(TM00_PSARR_b0) = klo;
}


void tm00_setup_cascade(uint16_t khi, uint16_t klo) {
  // Настройка режима работы
  RH(TM00_CR0_h0) =
    TM_CR0_MDS_cascade_h0 |   // включаем каскадный режим
    TM_CR0_EN2_enable_h0 |    // включаем CT2
    TM_CR0_EN_enable_h0;      // включаем CT1

  RB(TM00_ARR_b0) = khi;
  RB(TM00_PSARR_b0) = klo;
}


void tm00_setup_fullcnt(uint32_t k) {
  // Настройка режима работы
  RH(TM00_CR0_h0) =
    TM_CR0_MDS_full_counter_h0 | // включаем 16-битный режим
    TM_CR0_EN_enable_h0;      // включаем оба счетчика

  RB(TM00_ARR_b0) = k >> 8;
  RB(TM00_PSARR_b0) = (k & 0xff);
}


void tm_setup_separate(uint8_t tm_id, uint32_t mode, uint16_t khi, uint16_t klo) {
  register uint32_t ba = TM_BASE[tm_id]; // Base address
  // Настройка режима работы
  RW(ba + (TM20_CR0_w-TM20_Base)) =
    mode |
    TM_CR0_MDS_separate_w |  // включаем раздельный режим
    TM_CR0_EN2_enable_w |    // включаем CT2
    TM_CR0_EN_enable_w;      // включаем CT1
  RH(ba + (TM20_ARR_w-TM20_Base)) = khi;
  RH(ba + (TM20_PSARR_w-TM20_Base)) = klo;
}


void tm_setup_cascade(uint8_t tm_id, uint32_t mode, uint16_t khi, uint16_t klo) {
  register uint32_t ba = TM_BASE[tm_id]; // Base address
  RW(ba + (TM20_CR0_w-TM20_Base)) =
    mode |
    TM_CR0_MDS_cascade_w |   // включаем каскадный режим
    TM_CR0_EN2_enable_w |    // включаем CT2
    TM_CR0_EN_enable_w;      // включаем CT1
  RH(ba + (TM20_ARR_w-TM20_Base)) = khi;
  RH(ba + (TM20_PSARR_w-TM20_Base)) = klo;
}


void tm_setup_fullcnt(uint8_t tm_id, uint32_t mode, uint32_t k) {
  register uint32_t ba = TM_BASE[tm_id]; // Base address
  // Настройка режима работы
  RW(ba + (TM20_CR0_w-TM20_Base)) =
    mode |
    TM_CR0_MDS_full_counter_w |  // включаем полный режим
    TM_CR0_EN_enable_w;          // включаем счетчик

  RH(ba + (TM20_ARR_w-TM20_Base)) = k >> 16;
  RH(ba + (TM20_PSARR_w-TM20_Base)) = (k & 0xffff);
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

  //tm00_setup_separate(150-1,100-1); // K1 = 150, K2 = 100 => F1 = 10 kHz
  //tm00_setup_cascade(15-1,100-1); // K1 = 15, K2 = 100, Fout = 1 kHz:
  tm00_setup_fullcnt(1500); // K = 1500, Fout = 1 kHz:

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


void tm00_test_lowfreq_separate() {
  tm00_init();
  // Настройка тактирования: TM00_CK_TC2 - 4000 Гц
  RH(TM00_CLK_h0) =
    TM_CLK_CKI_DIV_div8_h0 | // TM00_CKI_DIV: DIV8
    TM_CLK_CKI_SEL_ck_ls_h0 | // TM00_CKI_SEL: CK_TMx_PR
    TM_CLK_CKS2_SEL_ck_int_h0 ;// TM00_CKS2_SEL: CK_INT

  tm00_setup_separate(255,255); // K1 = 150, K2 = 100 => F1 = 10 kHz

  while (1) {
    debug('C',RB(TM00_PSCNT_h0));
    delay_ms(10);
  }
}


void tm00_test_lowfreq_fullcnt() {
  uint8_t f;
  int i=0;
  tm_init(TM00);
  // Настройка тактирования: TM00_CK_TC2 - 4000 Гц
  RH(TM00_CLK_h0) =
    TM_CLK_CKI_DIV_div8_h0 | // TM00_CKI_DIV: DIV8
    TM_CLK_CKI_SEL_ck_ls_h0 | // TM00_CKI_SEL: CK_TMx_PR
    TM_CLK_CKS2_SEL_ck_int_h0 ;// TM00_CKS2_SEL: CK_INT

  tm00_setup_fullcnt(4000); // F1 = 1 Гц

  while (1) {
    debug('C',RH(TM00_PSCNT_h0));
    delay_ms(10);
    /*
    // Проверка Reset
    if (i++ == 5) {
      uart_puts(PORT,"RESET",UART_NEWLINE_CRLF);
      RB(TM00_TRG_b3) = TM_TRG_RST_SW_enable_b3; // reset on
      RB(TM00_TRG_b3) = 0; // reset off
      i=0;
    }
    */
    /*
    // Проверка Gate
    if (i%16 == 0) {
      uart_puts(PORT,"GATE1",UART_NEWLINE_CRLF);
      RB(TM00_TRG_b3) = TM_TRG_GT_SW_enable_b3; // gate sw on
    }
    if (i%16 == 8) {
      uart_puts(PORT,"GATE0",UART_NEWLINE_CRLF);
      RB(TM00_TRG_b3) = 0;
    }
    i++;
    */

    // Проверка флагов TOF и TOF2
    f=RB(TM00_STA_b0);
    if (f) {
      if (f & TM_STA_TOF_happened_b0) uart_puts(PORT,"TOF ",UART_NEWLINE_NONE);
      if (f & TM_STA_TOF2_happened_b0) uart_puts(PORT,"TOF2",UART_NEWLINE_NONE);
      uart_puts(PORT,"",UART_NEWLINE_CRLF);
      RB(TM00_STA_b0)=0xff;
    }

  }
}


void timer_test_tm1x() {
  tm_init(TM16_id);
  // Настройка тактирования:
  RW(TM16_CLK_w) = TM_CLK_CKI_DIV_div8_w | TM_CLK_CKI_SEL_ck_ls_w | TM_CLK_CKS2_SEL_ck_int_w;
  tm_setup_cascade(TM16_id,0,1024,1024);
  while (1) {
    //debug32('S',RW(TM20_STA_w));
    debug32('C',RW(TM16_PSCNT_w));
    //debug('C',RH(TM20_CNT_h0));
    delay_ms(100);
  }
}


void timer_hdl_tm26() {
  uart_puts(PORT,"IRQ",UART_NEWLINE_CRLF);
  RW(TM26_STA_w)=0xffffffff;
}


void timer_test_tm26() {
  uint32_t f;

  // Настройка TM00_TRGO в качестве источника 10 Гц
  tm_init(TM00_id);
  // Настройка тактирования: TM00_CK_TC2 - 4000 Гц
  RH(TM00_CLK_h0) = TM_CLK_CKI_DIV_div8_h0 | TM_CLK_CKI_SEL_ck_ls_h0 | TM_CLK_CKS2_SEL_ck_int_h0;
  tm00_setup_fullcnt(2000); // F1 = 2 Гц
  // TRGO <- UEV
  RW(TM00_TRG_w) = TM_TRG_TRGO_MDS_uev_w;
  // Контроль сигнала на выводе PD9 (39)
  RH(PD_CR9_h0) = (0x2 << 12) | 2; // PD9: TM00_TRGO, push pull output

  // Настройка TM26
  tm_init(TM26_id);
  // Настройка тактирования: TM26_CK_TC2 <- TM00_TRGO
  RW(TM26_CLK_w) = TM_CLK_CKS2_SEL_ck_ext_w | TM_CLK_CKE_SEL_itr_w;
  RB(TM26_TRG_b1) = TM_TRG_ITR_MUX_itr6_b1; // TM26: ITR6 = TRG0 = TM00_TRGO

  tm_setup_cascade(TM26_id,TM_CR0_DIR_up_w | TM_CR0_DIR2_up_w,5,5);
  //tm_setup_cascade(TM26_id,TM_CR0_DIR_down_w | TM_CR0_DIR2_down_w,5,5);
  //tm_setup_fullcnt(TM26_id,0,5);
  //tm_setup_fullcnt(TM26_id,TM_CR0_DIR_down_w,5);

  // Настройка прерывания
  SVC2(SVC_HANDLER_SET, TM_IRQ[TM26_id], timer_hdl_tm26); // устанавливаем обработчик прерывания
  tm_setup_int(TM26_id, TM_INT_TIE_enable_w | TM_INT_TIE2_enable_w);

  while (1) {
    //f=RW(TM26_STA_w);
    //if (f) {debug32('F',f); RW(TM26_STA_w)=0xffffffff;}
    __disable_irq();
    debug32('C',RW(TM26_PSCNT_w));
    __enable_irq();
    delay_ms(100);
  }

}


void timer_test_gen(uint16_t kfreq, uint16_t period, uint16_t pulse) {
  // Настройка выводов МК:
  RH(PC_CR0_h0) = (0x5 << 12) | 2; // PC0 (19): TM20_OC00, канал 0
  //RH(PA_CR1_h0) = (0xA << 12) | 2; // PA1 (46): TM36_OC10, канал 1
  //RH(PB_CR1_h0) = (0x7 << 12) | 2; // PB1 (10): TM36_TRGO, тактовый сигнал

  // Инициализация:
  tm_init(TM20_id);
  // Настройка тактирования: CK_TC2 <- 1.5 МГц
  RW(TM20_CLK_w) = TM_CLK_CKI_DIV_div8_w | TM_CLK_CKI_SEL_proc_h0 | TM_CLK_CKS2_SEL_ck_int_h0;
  // Настройка периодов:
  tm_setup_cascade(TM20_id,0,period-1,kfreq-1);

  // Пороги (полный цикл - 20):
  RH(TM20_CC0B_h0)=pulse;
  //RH(TM20_CC1B_h0)=10;

  // Настройка режима ШИМ:
  RB(TM20_CCMDS_b0) =
    TM_CCMDS_CC0_MDS_16bit_pwm_b0;// | // канал 0
    //TM_CCMDS_CC1_MDS_16bit_pwm_b0;  // канал 1

  // Включение выходов:
  RW(TM20_OCCR0_w) =
    TM_OCCR0_OC0_OE0_enable_w;// | // OC00 enable
    //TM_OCCR0_OC1_OE0_enable_w; // OC10 enable
}


/// Обработчик прерывания в режиме захвата по каналу 0
void timer_hdl_capture() {
  static uint32_t a;
  uint32_t b;
  uint32_t f=RW(TM26_STA_w);

  if (f & TM_STA_CF0A_happened_w) {
    b=a;
    a=(RH(TM26_CC0A_h0) << 16) | RH(TM26_CC0B_h0);
    debug32('A',a);
    debug32('T',a-b); // Период A1-A0
  }

  if (f & TM_STA_CF0B_happened_w) {
    b=(RH(TM26_CC0A_h0) << 16) | RH(TM26_CC0B_h0);
    debug32('B',b);
    debug32('P',b-a); // Длительность импульса B-A
    uart_puts(PORT,"--",UART_NEWLINE_CRLF);
  }
  RW(TM26_STA_w)=0xffffffff; // сброс всех флагов
}


/// Тестирование функции захвата на TM26
void timer_test_capture() {
  // Настройка TM00_TRGO в качестве источника 1 МГц

  tm_init(TM00_id);
  // Настройка тактирования: TM00_CK_TC2 - 12 МГц
  RH(TM00_CLK_h0) = TM_CLK_CKI_DIV_div1_h0 | TM_CLK_CKI_SEL_proc_h0 | TM_CLK_CKS2_SEL_ck_int_h0;
  tm00_setup_fullcnt(12-1); // F1 = 1 МГц
  // TRGO <- UEV
  RW(TM00_TRG_w) = TM_TRG_TRGO_MDS_uev_w;
  // Контроль сигнала на выводе PD9 (39)
  RH(PD_CR9_h0) = (0x2 << 12) | 2; // PD9: TM00_TRGO, push pull output

  // Настройка TM26

  tm_init(TM26_id);
  // Настройка тактирования: TM26_CK_TC2 <- TM00_TRGO
  RW(TM26_CLK_w) = TM_CLK_CKS2_SEL_ck_ext_w | TM_CLK_CKE_SEL_itr_w;
  RB(TM26_TRG_b1) = TM_TRG_ITR_MUX_itr6_b1; // TM26: ITR6 = TRG0 = TM00_TRGO

  tm_setup_fullcnt(TM26_id,TM_CR0_DIR_up_w,0xffffffff);

  // Настройка режима захвата на канале 0 по обоим фронтам от сигнала IC0
  RB(TM26_CCMDS_b0) = TM_CCMDS_CC0_MDS_16bit_ic_b0;
  RW(TM26_ICCR_w) = TM_ICCR_IC0_TRGS_dual_edge_w | TM_ICCR_IC0_MUX_ic00_w;  // IC0
  // Настройка входного сигнала TM26_IC0 на пин PB0
  RH(PB_CR0_h0) = (6 << 12) | (1 << 5) | 3; // TM26_IC0, digital input + pull-up

  // Настройка прерывания по событиям CC0A, CC0B
  SVC2(SVC_HANDLER_SET, 16, timer_hdl_capture); // устанавливаем обработчик прерывания
  tm_setup_int(TM26_id, TM_INT_CC0_IE_enable_w);

//  while (1) {
//    __disable_irq();
//    debug32('C',RW(TM26_PSCNT_w));
//    __enable_irq();
//    delay_ms(500);
//  }

}


/// Обработчик прерывания с таймера TM10
void timer_hdl_freq() {
  // Считываем значение частоты с TM16:
  debug32('F',RW(TM16_PSCNT_w));
  // Сбрасываем счетчик в TM16:
  RB(TM16_TRG_b3) |= TM_TRG_RST_SW_enable_b3;
  RB(TM16_TRG_b3) &= ~TM_TRG_RST_SW_enable_b3;
  // Сбрасываем флаг TOF и одновременно запускаем счетчик в TM10:
  RB(TM10_STA_b0)=0xff; //TM_STA_TOF_mask_b0;
}


/// Частотомер на базе TM10 и TM16
void timer_test_freq() {
  // Настройка TM10 как формирователя секундных импульсов

  tm_init(TM10_id);
  RH(TM10_CLK_h0) = TM_CLK_CKI_DIV_div4_h0 | TM_CLK_CKI_SEL_proc_h0 | TM_CLK_CKS2_SEL_ck_int_h0;
  // Включаем режим 32 бита с функцией автостопа:
  tm_setup_fullcnt(TM10_id, TM_CR0_ASTOP_EN_enable_h0, 2999806); // F(CKO1)=1 Гц
  //tm_setup_fullcnt(TM10_id, TM_CR0_ASTOP_EN_enable_h0, 3000000-1); // F(CKO1)=1 Гц
  // TRGO <- TOF
  RW(TM10_TRG_w) = TM_TRG_TRGO_MDS_tof_w;
  // Контроль сигнала на выводе PC12 (30)
  RH(PC_CR12_h0) = (5 << 12) | 2; // PC12: TM10_TRGO, push pull output
  // Настройка прерывания TM10 по событию TOF (TIE)
  SVC2(SVC_HANDLER_SET, 13, timer_hdl_freq); // устанавливаем обработчик прерывания
  tm_setup_int(TM10_id, TM_INT_TIE_enable_w);

  // Настройка выхода TM10_CKO
  //RH(PB_CR1_h0) = (0x4 << 12) | 2; // PB1: TM10_CKO, push pull output
  //RB(TM10_CKO_b0) = TM_CKO_CKO_SEL_main_b0 | TM_CKO_CKO_EN_enable_b0;

  // Настройка TM16 как основного счетчика входных импульсов

  tm_init(TM16_id);
  // Настройка тактирования: TM16_CK_TC2 <- TM16_ETR:
  RW(TM16_CLK_w) = TM_CLK_CKS2_SEL_ck_ext_w | TM_CLK_CKE_SEL_etr_w;
  // Настройка входного сигнала TM16_ETR на пин PB0:
  RH(PB_CR0_h0) = (5 << 12) | (1 << 5) | 3; // TM16_ETR, digital input + pull-up
  // Включаем режим 32 бит:
  tm_setup_fullcnt(TM16_id,TM_CR0_DIR_up_w,0xffffffff);
  // Включаем входной триггер ITR6=TM10_TRGO как Gate:
  RW(TM16_TRG_w)=
    TM_TRG_ITR_MUX_itr6_w |     // ITR <- ITR6
    TM_TRG_TRG_MUX_itr_w |      // TRGI <- ITR
    TM_TRG_TRGI_MDS_gate_h_w;   // TRGI_MDS: GATE HI
  // Включаем сигнал TM10_TRGO как ITR6 (TRG1=TM10_TRGO):
  RB(APB_CR2_b0)= APB_CR2_ITR6_MUX_trg1_b0;

}


/// Обработчик прерывания
void timer_hdl_pwm() {
  RH(PA_SC_h0) = 4; // set PA2
  __NOP(); __NOP();
  RH(PA_SC_h1) = 4; // clear PA2
  RB(TM36_STA_b0) = 0xFF; // clear flags
}


/// Генератор ШИМ-сигнала на базе TM36
void timer_test_pwm() {
  // Настройка выводов МК:
  RH(PA_CR0_h0) = (0xA << 12) | 2; // PA0 (45): TM36_OC00, канал 0
  RH(PA_CR1_h0) = (0xA << 12) | 2; // PA1 (46): TM36_OC10, канал 1
  RH(PA_CR2_h0) = (0x0 << 12) | 2; // PA2 (47): GPA2, software TOF
  RH(PB_CR1_h0) = (0x7 << 12) | 2; // PB1 (10): TM36_TRGO, тактовый сигнал

  // Инициализация TM36:
  tm_init(TM36_id);
  // Настройка тактирования: CK_TC2 <- 6 МГц
  RW(TM36_CLK_w) = TM_CLK_CKI_DIV_div2_w | TM_CLK_CKI_SEL_proc_w | TM_CLK_CKS2_SEL_ck_int_w;
  // TRGO <- UEV2
  RW(TM36_TRG_w) = TM_TRG_TRGO_MDS_uev2_w;
  // Настройка периодов: CT2 - 6000 (1 кГц), CT1 - 20 (50 Гц)
  tm_setup_cascade(TM36_id,TM_CR0_DIR_up_w,20-1,6000-1);
  //tm_setup_cascade(TM36_id,TM_CR0_DIR_down_w,20-1,6000-1);

  // Пороги (полный цикл - 20):
  RH(TM36_CC0B_h0)=5;
  RH(TM36_CC1B_h0)=10;

  // Начальный уровень сигналов OS0 и OS1
//  RB(TM36_OSCR_b0)=
//    TM_OSCR_OS1_LCK_un_locked_b0 | TM_OSCR_OS1_STA_1_b0 | // 1
//    TM_OSCR_OS0_LCK_un_locked_b0 | TM_OSCR_OS0_STA_1_b0; // 1

  // Настройка режима OC:
//  RB(TM36_CCMDS_b0) =
//    TM_CCMDS_CC0_MDS_16bit_oc_b0 | // канал 0
//    TM_CCMDS_CC1_MDS_16bit_oc_b0;  // канал 1

//  // Настройка режима ШИМ:
  RB(TM36_CCMDS_b0) =
    TM_CCMDS_CC0_MDS_16bit_pwm_b0 | // канал 0
    TM_CCMDS_CC1_MDS_16bit_pwm_b0;  // канал 1
  RH(TM36_PWM_h0) = TM_PWM_PWM_MDS_center_aligned_h0; // выравнивание по центру

  // Включение инверсии:
  //RW(TM36_OCCR1_w) = TM_OCCR1_OC0_INV_enable_w | TM_OCCR1_OC1_INV_enable_w;

  // Включение выходов:
  RW(TM36_OCCR0_w) =
    TM_OCCR0_OC0_OE0_enable_w | // OC00 enable
    TM_OCCR0_OC1_OE0_enable_w; // OC10 enable

  // Настройка прерывания
  SVC2(SVC_HANDLER_SET, 17, timer_hdl_pwm);   // устанавливаем обработчик прерывания
  tm_setup_int(TM36_id, TM_INT_TIE_enable_w); // прерывание по TOF/TUF
}
