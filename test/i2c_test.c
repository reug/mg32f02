#include "i2c_test.h"
#include "i2c.h"
#include "core.h"
#include "MG32x02z_I2C.h"
#include "hwcf.h"
#include "ulib.h"
#include "utils.h"
#include "ic/ds3231.h"

#define I2C_PORT I2C0_id

#define TMOUT_CHECK     if (i2c_get_tmout(id)) return;
#define TMOUT_CHECK2    if (i2c_get_tmout(id)) goto failure;


/// Буфер данных
#define BUFLEN 16
uint8_t  buf[BUFLEN]; ///< данные буфера
uint32_t bufp;        ///< указатель буфера
uint32_t bufn;        ///< размер данных на отправку


void i2c_test_master_setup() {
  i2c_init(DS3231_PORT);
  HW_I2C0_SETSCL;
  HW_I2C0_SETSDA;

  // Настройка режима работы
  i2c_setup_mode(DS3231_PORT,
      I2C_CR0_PDRV_SEL_1t_w |
      I2C_CR0_BUF_EN_enable_w |   // Режим работы через буфер
      I2C_CR0_MDS_i2c_w           // I2C : Single/Multi-Master/ Slave mode
  );
}



void i2c_test_master_w1r_ds3231() {
  uint32_t d;
  while (1) {
    d=clock_get_bcd(); //d=ds3231_read_multi(10,10);
    if (i2c_get_tmout(I2C_PORT)) {
      d=i2c_get_status(I2C_PORT);
      debug32hex('S',d); i2c_print_status(d);
      led2_flash();
      i2c_clr_status(I2C_PORT, I2C_STA_TMOUTF_mask_w);
    }
    else {
      debug32hex('T',d);
    }
    delay_ms(1000);
  }
}


void i2c_test_master_wN(uint8_t n) {
  uint32_t d;
  uint8_t buf[]={0xA1,0xB2,0xC3,0xD4,0xE5,0xF6};

  while (1) {
    ds3231_write_multi(1,n,*(uint32_t*)buf);
    if (i2c_get_tmout(I2C_PORT)) {
      d=i2c_get_status(I2C_PORT);
      debug32hex('S',d); i2c_print_status(d);
      led2_flash();
      i2c_clr_status(I2C_PORT, I2C_STA_TMOUTF_mask_w);
    }
    else {
      uart_puts(PORT,"OK",UART_NEWLINE_CRLF);
    }
    delay_ms(1000);
  }

}


void i2c_test_master_req(uint32_t id, uint8_t addr, void* buf, uint8_t nbytes) {
  uint8_t i=0,n;
  uint32_t d;
  // Отправка запроса:
  i2c_master_startw(id, addr); TMOUT_CHECK
  i2c_master_send(id, I2C_START, 1, nbytes); TMOUT_CHECK
  i2c_wait_start(id); TMOUT_CHECK
  // Получение ответа:
  i2c_master_startr(id, addr); TMOUT_CHECK
  while ( (n = nbytes-i) > 0 ) {
    if (n > 4) {
      n=4;
      d=i2c_master_recv(id, I2C_NOOPTS, 4); TMOUT_CHECK
    }
    else {
      d=i2c_master_recv(id, I2C_STOP, n); TMOUT_CHECK
    }
    *(uint32_t*)((uint8_t*)buf+i)=d;
    i += n;
  }
  i2c_wait_stop(id);
}


void i2c_test_master_w1r() {
  uint32_t d;
  uint8_t n=9;
  while (1) {
    i2c_test_master_req(I2C_PORT,DS3231_ADDR,buf,n);
    d=i2c_get_status(I2C_PORT);
    if (d & I2C_STA_TMOUTF_mask_w) {
      debug32hex('S',d); i2c_print_status(d);
      led2_flash();
      i2c_clr_status(I2C_PORT, I2C_STA_TMOUTF_mask_w);
    }
    else {
      debugbuf(buf,n);
    }
    delay_ms(1000);
  }
}
