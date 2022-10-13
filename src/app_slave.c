// RAM based application framework for ARM Cortex-M0
// by reug@mail.ru, 2022

#include "core.h"
#include "hwcf.h" // Аппаратная конфигурация
#include "ulib.h"
#include "utils.h"
#include "init.h"
#include "i2c.h"
#include "ic/ds3231.h"


// Mark first word with signature "Application is present" (nop; nop: 0x46c046c0)
__attribute__ ((section (".app_sign"))) __attribute__ ((__used__))
static uint32_t  app_sign=APP_SIGNATURE;


/// Тест режима мастера на MG32F02A032
void i2c_test_master() {
  uint32_t d;

  i2c_init(DS3231_PORT);
  // Настройка выводов I2C:
  HW_I2C0_SETSCL;
  HW_I2C0_SETSDA;

  // Настройка тактирования
  i2c_setup_clock(DS3231_PORT,
      I2C_CLK_TMO_CKS_div64_h0 |  // CK_TMO: F(CK_PSC)/64 = 12500 Hz
      ((15 -1) << I2C_CLK_CK_PSC_shift_h0) | // CK_PSC: 12 MHz /15 = 800 kHz
      I2C_CLK_CK_DIV_div8_h0 |    // CK_I2Cx_INT: 200 kHz => F = 50 kHz
      I2C_CLK_CK_SEL_proc_h0      // I2Cx_CK_SEL: APB, 12 MHz
  );
  // Тайминг режима master
  //RW(I2C0_CR1_w) = 0x0202;
  // Настройка режима работы
  i2c_setup_mode(DS3231_PORT,
      I2C_CR0_PDRV_SEL_1t_w |
      I2C_CR0_BUF_EN_enable_w |   // Режим работы через буфер
      I2C_CR0_MDS_i2c_w           // I2C : Single/Multi-Master/ Slave mode
  );

  //time_set_v2(0x162345);
  //return;
  while (1) {
    d=clock_get_bcd();
    if (i2c_get_tmout(DS3231_PORT)) {
      d=i2c_get_status(DS3231_PORT);
      debug32hex('S',d); i2c_print_status(d);
      led2_flash();
      i2c_clr_status(DS3231_PORT, I2C_STA_TMOUTF_mask_w);
    }
    else debug32hex('T',d);
    delay_ms(500);
  }
}


#define I2C_PORT I2C0_id
/*
void i2c_slave_ack(uint32_t id) {
  RB(id+( I2C0_CR2_b0 -I2C0_Base)) =
    (I2C_CR2_AA_LCK_un_locked_b0 | I2C_CR2_AA_mask_b0);
    //(I2C_CR2_CMD_TC_enable_b0 | I2C_CR2_AA_LCK_un_locked_b0 | I2C_CR2_PAA_mask_b0);
}

void i2c_slave_ans(uint32_t id, uint8_t len) {
//  RB(id+( I2C0_CR2_b0 -I2C0_Base)) =
//    (I2C_CR2_CMD_TC_enable_b0 | I2C_CR2_AA_LCK_un_locked_b0 | I2C_CR2_AA_mask_b0);
    //(I2C_CR2_CMD_TC_enable_b0 | I2C_CR2_AA_LCK_un_locked_b0 | I2C_CR2_PAA_mask_b0);
  RW(id+( I2C0_CR2_w -I2C0_Base)) =
    ((len & 0x07) << 8) |// | // BUF_CNT
    (I2C_CR2_CMD_TC_enable_w | I2C_CR2_AA_LCK_un_locked_w | I2C_CR2_PAA_mask_w);
    //(I2C_CR2_AA_LCK_un_locked_w | I2C_CR2_AA_mask_w);
}

/// Чтение данных из буфера. Возвращает фактическое число скопированных из буфера байт.
uint8_t i2c_slave_read(uint32_t id, uint8_t* buf, uint8_t len) {
  uint8_t i,n = RB(id+( I2C0_CR2_b2 -I2C0_Base)) & 0x07; // ACNT
  uint32_t d = RW(id+( I2C0_DAT_w -I2C0_Base));
  if (n>len) n=len;
  for (i=0; i<n; i++) {
    buf[i]= d & 0xFF; d >>= 8;
  }
  return n;
}
*/

/// Запись данных в буфер отправки
inline
void i2c_write(uint32_t id, uint32_t data, uint8_t len) {
  RB(id+( I2C0_CR2_b1 -I2C0_Base)) = (len & I2C_CR2_BUF_CNT_mask_b1); // BUF_CNT
  RW(id+( I2C0_DAT_w -I2C0_Base)) = data;
}


/// Запись данных в буфер.
/// Возвращает оставшееся число байт.
uint32_t i2c_writebuf(uint32_t id, void* buf, uint32_t* p, uint32_t len) {
  uint8_t m;
  int32_t n;
  n = len - *p;
  if (n > 0) {
    m = n < 4 ? n : 4;
    RB(id+( I2C0_CR2_b1 -I2C0_Base)) = (m & I2C_CR2_BUF_CNT_mask_b1); // BUF_CNT
    RW(id+( I2C0_DAT_w -I2C0_Base)) = *(uint32_t*)((uint8_t*)buf + *p);
    *p += m;
  }
  return len-*p;
}


/// Чтение буфера приема (4 байта)
inline
uint32_t i2c_read(uint32_t id) {
  return RW(id+( I2C0_DAT_w -I2C0_Base));
}


/// Чтение принятых данных и их запись в указанный буфер.
/// На входе: *p - текущее положение в буфере (указывает на следующий байт после записанного).
/// На выходе: *p - новое значение, увеличенное на число считанных байт.
/// Возвращает фактическое число считанных байт.
uint8_t i2c_readbuf(uint32_t id, void* buf, uint32_t* p) {
  uint8_t n = RB(id+( I2C0_CR2_b1 -I2C0_Base)) & I2C_CR2_BUF_CNT_mask_b1; // BUF_CNT
  *(uint32_t*)((uint8_t*)buf + *p) = RW(id+( I2C0_DAT_w -I2C0_Base));
  *p += n;
  return n;
}



#define BUFLEN 8

/// Буфер данных слэйва
uint8_t  buf[BUFLEN]; ///< данные буфера
uint32_t bufp;        ///< указатель буфера
uint32_t bufn;        ///< размер данных на отправку


/*
/// Состояние слэйва
struct SlaveState {
  uint8_t st;   // FSM state
  uint8_t bn;   // номер отправленного или принятого байта
  uint8_t reg;  // запрашиваемый регистр
} ss;
*/

/// Обработчик прерывания I2C0
void i2c_hdl_w1rN() {
  uint32_t d; // флаги
  uint32_t n;

  led1_on();
  d=i2c_get_status(I2C_PORT);

  if (d & I2C_STA_SADRF_mask_w) {
    if (d & I2C_STA_RWF_read_w) { // Master reads
      //led2_on();
    }
    else { // Master writes
      //led2_on();
      bufp=0; bufn=0;
    }
  }
  if (d & I2C_STA_TXF_mask_w) {
    if (bufp==bufn) led2_on();
    if (bufn > bufp) i2c_writebuf(I2C_PORT,buf,&bufp,bufn);

  }
  if (d & I2C_STA_RXF_mask_w) {
    n=i2c_read(I2C_PORT);
    //if (n==6) led2_on();
    bufn = (n <= BUFLEN) ? n : BUFLEN;
    i2c_writebuf(I2C_PORT,buf,&bufp,bufn); // Подготавливаем данные для отправки (копируем в буфер максимум байт (4))
    //led2_on();
    //if (bufp==4) led2_on();
  }
  if (d & I2C_STA_STOPF_mask_w) {
    //led2_on();
  }
  if (d & I2C_STA_RSTRF_mask_w) {
    //led2_on();
  }
  led1_off(); led2_off();
  //i2c_clr_status(I2C_PORT, I2C_STA_BUFF_mask_w);
  //i2c_clr_status(I2C_PORT, d);
  i2c_clr_status(I2C_PORT, 0x00ffffff);
}


/// Обработчик прерывания I2C0
void i2c_hdl_wN() {
  uint32_t n; // число байт на отправку
  uint32_t d;

  led1_on();
  d=i2c_get_status(I2C_PORT);

  if (d & I2C_STA_SADRF_mask_w) {
    if (d & I2C_STA_RWF_read_w) { // Master reads
      //led2_on();
    }
    else { // Master writes
      //led2_on();
      bufp=0;
    }
  }
  if (d & I2C_STA_TXF_mask_w) {
    //led2_on();
  }
  if (d & I2C_STA_RXF_mask_w) {
    n=i2c_readbuf(I2C_PORT,buf,&bufp);
    if ( bufp== 5) led2_on();
    //led2_on();
  }
  if (d & I2C_STA_STOPF_mask_w) {
    //led2_on();
    debugbuf(buf,bufp);
    //bufp=0;
  }
  if (d & I2C_STA_RSTRF_mask_w) {
    //led2_on();
  }
  led1_off(); led2_off();
  //i2c_clr_status(I2C_PORT, I2C_STA_BUFF_mask_w);
  //i2c_clr_status(I2C_PORT, d);
  i2c_clr_status(I2C_PORT, 0x00ffffff);
  //i2c_clr_status(I2C_PORT, ~I2C_STA_EVENTF_mask_w);
}

/// Общая настройка режима слэйва
void i2c_test_slave() {
  uint32_t i;

  //ss.st=0; ss.bn=0; ss.reg=0x77;
  for (i=0; i< BUFLEN; i++) buf[i]=((i+1)<< 4) | (i+1);

  i2c_init(I2C_PORT);
  // Настройка выводов I2C:
  HW_I2C0_SETSCL;  HW_I2C0_SETSDA;
  // Настройка тактирования
  i2c_setup_clock(I2C_PORT,
      I2C_CLK_TMO_CKS_div64_h0 |  // CK_TMO: F(CK_PSC)/64 = 12500 Hz
      ((15 -1) << I2C_CLK_CK_PSC_shift_h0) | // CK_PSC: 12 MHz /15 = 800 kHz
      I2C_CLK_CK_DIV_div8_h0 |    // CK_I2Cx_INT: 200 kHz => F = 50 kHz
      I2C_CLK_CK_SEL_proc_h0      // I2Cx_CK_SEL: APB, 12 MHz
  );
  // Настройка режима работы
  i2c_setup_mode(I2C_PORT,
      I2C_CR0_PDRV_SEL_1t_w |
      I2C_CR0_BUF_EN_enable_w |   // Режим работы через буфер
      I2C_CR0_MDS_i2c_w |         // I2C : Single/Multi-Master/ Slave mode
      I2C_CR0_SADR_EN_enable_b0   // Включаем детектор адреса слэйва
  );
  // Адрес слэйва:
  RB(I2C0_SADR_b0) = DS3231_ADDR; // установка адреса
  RB(I2C0_MASK_b0) = 0xFE; // настройка маски

  // Отключаем задержку сигнала SCL от слэйва
  RB(I2C0_CR0_b1) |= I2C_CR0_SCLS_DIS_disable_b1;

  // Устанавливаем обработчик прерываний:
  SVC2(SVC_HANDLER_SET,28,i2c_hdl_w1rN);
  // Включаем прерывания в модуле:
  RW(I2C0_INT_w) =
    I2C_INT_BUF_IE_enable_w | // flags: RXF, TXF, RSTRF, STOPF, SADRF
    I2C_INT_IEA_enable_w;
  // Включаем прерывание в модуле NVIC:
  RW(CPU_ISER_w) = (1 << 28); // SETENA 28

  while (1) {
    // Проверка ACNT:
    //if (RB(I2C0_CR2_b2) & 0x07) led2_on();
    // Проверка CNTF:
    //if (RW(I2C0_STA_w) & I2C_STA_EVENTF_mask_w) {led2_on(); __NOP(); led2_off();}
  }

}


// First function in application
__attribute__ ((section(".app"))) // put function in the begin of .text after signature word "app_sign"
__attribute__ ((noreturn))
void app() {

  // Настройка выводов для LED D1, D2:
  RH(HW_LED1_CRH0) = 0x0002; // pin -> push-pull output
  RH(HW_LED2_CRH0) = 0x0002; // pin -> push-pull output
  // Выключаем светодиоды:
  RH(HW_LED1_SCH1) = HW_LED1_MASK;
  RH(HW_LED2_SCH1) = HW_LED2_MASK;

  // Настройка выводов URT0:
  HW_URT0_SETTX;  HW_URT0_SETRX;

  uart_init(PORT);
  uart_puts(PORT,"Hello",UART_NEWLINE_CRLF);
  //i2c_test_master();
  i2c_test_slave();

  while (1) ;
}
