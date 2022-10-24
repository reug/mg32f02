#include "enc28j60.h"
//#include "hwcf.h"
//#include "core.h"
#include "spi.h"
#include "ulib.h"


volatile uint8_t enc28j60_current_bank = 0;
volatile uint16_t enc28j60_rxrdpt;


void enc28j60_select() {
  spi_nss(1);
}

void enc28j60_release()  {
  spi_nss(0);
}

uint8_t enc28j60_rx() {
  return spi_rx();
}

void enc28j60_tx(uint8_t data) {
  spi_tx(data);
}


// Generic SPI read command
uint8_t enc28j60_read_op(uint8_t cmd, uint8_t adr) {
  uint8_t data;

  enc28j60_select();
  enc28j60_tx(cmd | (adr & ENC28J60_ADDR_MASK));
  if(adr & 0x80) // throw out dummy byte
    enc28j60_rx(); // when reading MII/MAC register
  data = enc28j60_rx();
  enc28j60_release();
  return data;
}

// Generic SPI write command
void enc28j60_write_op(uint8_t cmd, uint8_t adr, uint8_t data) {
  enc28j60_select();
  enc28j60_tx(cmd | (adr & ENC28J60_ADDR_MASK));
  enc28j60_tx(data);
  enc28j60_release();
}

// Initiate software reset
void enc28j60_soft_reset() {
  enc28j60_select();
  enc28j60_tx(ENC28J60_SPI_SRC);
  enc28j60_release();

  enc28j60_current_bank = 0;
  delay_ms(1); // Wait until device initializes
}


/*
 * Memory access
 */

// Set register bank
void enc28j60_set_bank(uint8_t adr) {
  uint8_t bank;

  if( (adr & ENC28J60_ADDR_MASK) < ENC28J60_COMMON_CR ) {
    bank = (adr >> 5) & 0x03; //BSEL1|BSEL0=0x03
    if(bank != enc28j60_current_bank) {
      enc28j60_write_op(ENC28J60_SPI_BFC, ECON1, 0x03);
      enc28j60_write_op(ENC28J60_SPI_BFS, ECON1, bank);
      enc28j60_current_bank = bank;
    }
  }
}

// Read register
uint8_t enc28j60_rcr(uint8_t adr) {
  enc28j60_set_bank(adr);
  return enc28j60_read_op(ENC28J60_SPI_RCR, adr);
}

// Read register pair
uint16_t enc28j60_rcr16(uint8_t adr) {
  enc28j60_set_bank(adr);
  return enc28j60_read_op(ENC28J60_SPI_RCR, adr) |
         (enc28j60_read_op(ENC28J60_SPI_RCR, adr+1) << 8);
}

// Write register
void enc28j60_wcr(uint8_t adr, uint8_t arg) {
  enc28j60_set_bank(adr);
  enc28j60_write_op(ENC28J60_SPI_WCR, adr, arg);
}

// Write register pair
void enc28j60_wcr16(uint8_t adr, uint16_t arg) {
  enc28j60_set_bank(adr);
  enc28j60_write_op(ENC28J60_SPI_WCR, adr, arg);
  enc28j60_write_op(ENC28J60_SPI_WCR, adr+1, arg>>8);
}


void enc28j60_bfc(uint8_t adr, uint8_t mask) {
  enc28j60_set_bank(adr);
  enc28j60_write_op(ENC28J60_SPI_BFC, adr, mask);
}


void enc28j60_bfs(uint8_t adr, uint8_t mask) {
  enc28j60_set_bank(adr);
  enc28j60_write_op(ENC28J60_SPI_BFS, adr, mask);
}


void enc28j60_read_buffer(uint8_t *buf, uint16_t len) {
  enc28j60_select();
  enc28j60_tx(ENC28J60_SPI_RBM);
  while(len--)
    *(buf++) = enc28j60_rx();
  enc28j60_release();
}


void enc28j60_write_buffer(uint8_t *buf, uint16_t len) {
  enc28j60_select();
  enc28j60_tx(ENC28J60_SPI_WBM);
  while(len--)
    enc28j60_tx(*(buf++));
  enc28j60_release();
}

// Read PHY register
uint16_t enc28j60_read_phy(uint8_t adr) {
  enc28j60_wcr(MIREGADR, adr);
  enc28j60_bfs(MICMD, MICMD_MIIRD);
  while(enc28j60_rcr(MISTAT) & MISTAT_BUSY)
    ;
  enc28j60_bfc(MICMD, MICMD_MIIRD);
  return enc28j60_rcr16(MIRD);
}

// Write PHY register
void enc28j60_write_phy(uint8_t adr, uint16_t data) {
  enc28j60_wcr(MIREGADR, adr);
  enc28j60_wcr16(MIWR, data);
  while(enc28j60_rcr(MISTAT) & MISTAT_BUSY)
    ;
}

