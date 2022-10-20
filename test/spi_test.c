#include "spi_test.h"
#include "core.h"
#include "MG32x02z_SPI.h"
#include "hwcf.h"
#include "ulib.h"
#include "utils.h"
#include "ic/enc28j60.h"


//#define TMOUT_CHECK     if (i2c_get_tmout(id)) return;
//#define TMOUT_CHECK2    if (i2c_get_tmout(id)) goto failure;


/// Буфер данных
#define BUFLEN 2048
uint8_t  buf[BUFLEN]; ///< данные буфера
uint32_t bufp;        ///< указатель буфера
uint32_t bufn;        ///< размер данных на отправку


void spi_test_master() {
  // Настройка выводов:
  HW_SPI0_SETMISO;  HW_SPI0_SETMOSI;  HW_SPI0_SETSCK;  HW_SPI0_SETNSS;
  // Инициализация, настройка тактирования:
  spi_init();

  // Настройка режима работы
  spi_setup_mode(
    SPI_CPOL_LOW | SPI_CPHA_LE | SPI_MSB
  );

  while (1) {
    ;
  }
}

