#include "spi_test.h"
#include "core.h"
#include "MG32x02z_SPI.h"
#include "hwcf.h"
#include "ulib.h"
#include "utils.h"
#include "ic/enc28j60.h"


//#define TMOUT_CHECK     if (i2c_get_tmout(id)) return;
//#define TMOUT_CHECK2    if (i2c_get_tmout(id)) goto failure;

/// Local MAC address
const uint8_t addr[6]={0x02,0xEE,0x10,0x00,0x00,0x01};

/// Destination MAC address
//uint8_t dest[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


void pin_test(uint8_t p) {
  RH(PA_CR8_h0) = 2;  RH(PA_CR9_h0) = 2;  RH(PA_CR10_h0) = 2;  RH(PA_CR11_h0) = 2;
  while (1) {
    RH(PA_SC_h0) = (1 << p);  delay_ms(1);
    RH(PA_SC_h1) = (1 << p);  delay_ms(1);
  }
}



/// Буфер данных
#define BUFLEN 1536
uint8_t  buf[BUFLEN]; ///< данные буфера
uint32_t bufp;        ///< указатель буфера
uint32_t bufn;        ///< размер данных на отправку


void spi_test_master() {
  uint32_t d;
  //pin_test(8);

  // Настройка выводов:
  HW_SPI0_SETMISO;  HW_SPI0_SETMOSI;  HW_SPI0_SETSCK;  HW_SPI0_SETNSS;

  // Инициализация, настройка тактирования:
  spi_init();

  // Настройка режима работы
  spi_setup_mode(
    SPI_NSSO_INV | SPI_NSS_SWEN | // software NSS control
    SPI_NSSO_EN | SPI_MASTER | SPI_MSB | SPI_CPHA_LE | SPI_CPOL_LOW
    | SPI_CR0_DOUT_MDS_enable_w // надо включить, если нет резистора подтяжки
  );

  RW(SPI0_CR2_w) |=
   (8 << SPI_CR2_DSIZE_shift_w); // Размер кадра в битах

  //eth_init(addr);

  spi_nss(1);   spi_tx(0xff);  spi_nss(0);  delay_ms(100);

  while (1) {

    //spi_nss(1);
    //d = enc28j60_rcr(ESTAT);
    d = enc28j60_rcr(ECON2);
    //spi_nss(0);
    debug('R',d);
    delay_ms(100);
  }

  //enc28j60_soft_reset();
  return;
//
//
// while (1) {
//  RB(SPI0_CR2_b3) = 1; //spi_nss(1);
//  spi_tx(0xff);
//  while (! (RW(SPI0_STA_w) & SPI_STA_TCF_happened_w)) ;
//  RB(SPI0_CR2_b3) = 0; //spi_nss(0);
//
//    delay_ms(100);
// }
}

