#include "adc_test.h"
#include "adc.h"
#include "ulib.h"
#include "utils.h"

uint32_t af; // adc flags
uint16_t ad[16]; // adc results


void adc_test_one() {
  uint16_t d;
  char s[8];
  adc_init();
  //adc_vbuf();
  //adc_tso(0);
  while (1) {
    led1_flash();
    adc_start_one(8);
    d=adc_read();
    strUint16(s,5,d);
    uart_puts(PORT,s,UART_NEWLINE_CRLF);
    delay_ms(500);
  }
}


void adc_hdl_scan() {
  *(volatile uint8_t*)ADC0_STA_b0 = 0x08;  // clear ADC0_E1CNVF flag
  uint32_t c,d;
  d = *(volatile uint32_t*)ADC0_DAT0_w;
  c = d >> 28; // ADC0_DAT0_CH
  af |= (1 << c);
  ad[c]=d; // ADC0_DAT0
}


void adc_test_scan() {
  uint32_t i;
  adc_init();
  SVC2(SVC_HANDLER_SET,10,adc_hdl_scan); // устанавливаем обработчик прерываний
  // включаем прерывания в модуле ADC:
  *(volatile uint16_t*)ADC0_INT_h0 = (1 << 3) | 1; // ADC0_E1CNV_IE | ADC0_IEA
  // включаем прерывание в модуле NVIC:
  *(volatile uint32_t*)CPU_ISER_w = (1 << 10); // SETENA 10
  // Увеличиваем время сэмплирования:
  //*(volatile uint8_t*)ADC0_CR0_b2 = 20; // ADC0_SMP_SEL
  while (1) {
    af=0; for (i=0; i<16; i++) ad[i]=0; // init
    led1_flash();
    adc_start_mask(0x0f00); // выбираем каналы 8-11
    delay_ms(200);
    uart_puts(PORT,"------------",UART_NEWLINE_CRLF);
    if (*(volatile uint8_t*)ADC0_STA_b0 & (1 << 5)) { // ADC0_ESCNVF==1
      uart_puts(PORT,"ESCNVF",UART_NEWLINE_CRLF);
    }
    debug('M',af);
    debug('8',ad[8]);
    debug('9',ad[9]);
    debug('A',ad[10]);
    debug('B',ad[11]);
  }
}


void adc_test_sum() {
  char s[8];
  adc_init();
  while (1) {
    led1_flash();
    strUint16(s,5,adc_measure_sum(8));
    uart_puts(PORT,s,UART_NEWLINE_CRLF);
    delay_ms(500);
  }
}


void adc_test_sum_cont() {
  uint16_t d;
  char s[8];
  adc_init();
  while (1) {
    adc_start_sum0(8); // Result sum
    // Ожидаем заданное число измерений по активации флага ADC0_SUMCF
    while (! ( *(volatile uint16_t*)ADC0_STA_h0 & (1 << 14) ) ); // ADC0_SUMCF == 1 ?
    d = adc_read_sum0();

    // Контроль пределов суммы:
    if (d<16000) {
      *(volatile uint16_t*)PB_SC_h0 = (1 << 13); // set bit 14
    }
    if (d>17000) {
      *(volatile uint16_t*)PB_SC_h0 = (1 << 14); // set bit 14
    }
    strUint16(s,5,d);
    uart_puts(PORT,s,UART_NEWLINE_CRLF);
    delay_ms(100);
  }
}


void adc_test_ivr24() {
  char s[8];
  adc_init();
  adc_ivr24();
  while (1) {
    led1_flash();
    strUint16(s,5,adc_measure_sum(8));
    uart_puts(PORT,s,UART_NEWLINE_CRLF);
    delay_ms(500);
  }
}


void cmp_test() {
  cmp_init(0);
  // Входы: PA8 -> CMP0_I0 (pos), PA9 -> CMP0_I1 (neg)
  // Выход (PC9):
  *(volatile uint16_t*)PC_CR9_h0 = (1 << 12) | 2; // AFS: CMP0_P0, PC9 -> push-pull output
  *(volatile uint32_t*)CMP_CR0_w =
    (0 << 18)  | //CMP_AC0_FSEL: bypass
    (0 << 12)  | // CMP_AC0_NMUX: IVREF     //(2 << 12)  | // CMP_AC0_NMUX: CMP0_I1
    (1 << 8)   | // CMP_AC0_PMUX: CMP0_I0
    (1 << 4)   | // CMP_AC0_HYS = 1, 10 mV
    1; // CMP_AC0_EN = 1
  *(volatile uint8_t*)CMP_ANA_b0 =
    (63 << 2) | // CMP_IVREF_RS = 63
     3; // CMP_IVREF_SEL =1: VR0 (1.650 V), CMP_IVREF_EN = 1

}


void cmp_test_ivref() {
  cmp_init(0);
  // Входы: PA8 -> CMP0_I0 (pos), PA9 -> CMP0_I1 (neg)
  *(volatile uint32_t*)CMP_CR0_w =
    (1 << 31)  | // CMP_AC0_IVROE = 1
    (2 << 12)  | // CMP_AC0_NMUX: CMP0_I1
    (1 << 8)   | // CMP_AC0_PMUX: CMP0_I0
    1; // CMP_AC0_EN = 1
  *(volatile uint8_t*)CMP_ANA_b0 =
    (31 << 2) | // CMP_IVREF_RS = 63
    1;// 3; // CMP_IVREF_SEL = 0: VDD (3.3 V), CMP_IVREF_EN = 1
}


void cmp_test_ivref_gen() {
  uint8_t i;
  cmp_init(0);
  // Входы: PA8 -> CMP0_I0 (pos), PA9 -> CMP0_I1 (neg)
  *(volatile uint32_t*)CMP_CR0_w =
    (1 << 31)  | // CMP_AC0_IVROE = 1
    (2 << 12)  | // CMP_AC0_NMUX: CMP0_I1
    (1 << 8)   | // CMP_AC0_PMUX: CMP0_I0
    1; // CMP_AC0_EN = 1

  while (1) {
    for (i=0; i<=63; i++) {
      *(volatile uint8_t*)CMP_ANA_b0 = (i << 2) | 1; // CMP_IVREF_EN = 1
      // Подбор временного масштаба для осциллографа:
      __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    }
  }
}

