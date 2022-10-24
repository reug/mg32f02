#include "ethernet.h"
#include "ic/enc28j60.h"


volatile uint8_t eth_frame[ETH_FRAME_MAXSIZE] __attribute__ ((aligned(4)));
volatile uint16_t eth_frame_len;


void eth_setframe_addr(const uint8_t* addr) {
  uint8_t i;
  for (i=0; i<6; i++) eth_frame[i+6]=addr[i];
}


void eth_setframe_dest(const uint8_t* dest) {
  uint8_t i;
  for (i=0; i<6; i++) eth_frame[i]=dest[i];
}


void eth_setframe_size(uint16_t len) {
  eth_frame[12]=(len >> 8) & 0xFF;
  eth_frame[13]=len & 0xFF;
}


void eth_setframe_swap() {
  uint8_t b,i,j;
  for (i=0; i<6; i++) {
    j=i+6;
    b=eth_frame[i]; eth_frame[i]=eth_frame[j]; eth_frame[j]=b;
  }
}



/*
 * Init & packet Rx/Tx
 */

void eth_init(const uint8_t *macadr) {
  // Reset ENC28J60
  enc28j60_soft_reset();

  // Setup Rx/Tx buffer
  enc28j60_wcr16(ERXST, ENC28J60_RXSTART);
  enc28j60_wcr16(ERXRDPT, ENC28J60_RXSTART);
  enc28j60_wcr16(ERXND, ENC28J60_RXEND);
  enc28j60_rxrdpt = ENC28J60_RXSTART;

  // Setup MAC
  enc28j60_wcr(MACON1, MACON1_TXPAUS| // Enable flow control
               MACON1_RXPAUS|MACON1_MARXEN); // Enable MAC Rx
  enc28j60_wcr(MACON2, 0); // Clear reset
  enc28j60_wcr(MACON3, MACON3_PADCFG0| // Enable padding,
               MACON3_TXCRCEN|MACON3_FRMLNEN|MACON3_FULDPX); // Enable crc & frame len chk
  enc28j60_wcr16(MAMXFL, ENC28J60_MAXFRAME);
  enc28j60_wcr(MABBIPG, 0x15); // Set inter-frame gap
  enc28j60_wcr(MAIPGL, 0x12);
  enc28j60_wcr(MAIPGH, 0x0c);

  enc28j60_wcr(MAADR5, macadr[0]); // Set MAC address
  enc28j60_wcr(MAADR4, macadr[1]);
  enc28j60_wcr(MAADR3, macadr[2]);
  enc28j60_wcr(MAADR2, macadr[3]);
  enc28j60_wcr(MAADR1, macadr[4]);
  enc28j60_wcr(MAADR0, macadr[5]);

  // Setup PHY
  enc28j60_write_phy(PHCON1, PHCON1_PDPXMD); // Force full-duplex mode
  enc28j60_write_phy(PHCON2, PHCON2_HDLDIS); // Disable loopback
  enc28j60_write_phy(PHLCON, PHLCON_LACFG2| // Configure LED ctrl
                     PHLCON_LBCFG2|PHLCON_LBCFG1|PHLCON_LBCFG0|
                     PHLCON_LFRQ0|PHLCON_STRCH);

  // Enable Rx packets
  enc28j60_bfs(ECON1, ECON1_RXEN);
}


void eth_setup_int() {
  // ENC28J60 interrupt enable:
  // bit 7: INTIE=1 - Allow interrupt events to drive the INT pin
  // bit 6: PKTIE=1 - Enable receive packet pending interrupt
  enc28j60_wcr(EIE,0b11000000);
}


void eth_clear_int() {
  // bit 6: PKTIF=1 - Receive Packet Pending Interrupt Flag bit
  //enc28j60_wcr(EIR,0b01000000);
  enc28j60_bfc(EIR,0b01000000);
}


void eth_sendpkt(uint8_t *data, uint16_t len) {
  while(enc28j60_rcr(ECON1) & ECON1_TXRTS) {
    // TXRTS may not clear - ENC28J60 bug. We must reset
    // transmit logic in cause of Tx error
    if(enc28j60_rcr(EIR) & EIR_TXERIF) {
      enc28j60_bfs(ECON1, ECON1_TXRST);
      enc28j60_bfc(ECON1, ECON1_TXRST);
    }
  }

  enc28j60_wcr16(EWRPT, ENC28J60_TXSTART);
  enc28j60_write_buffer((uint8_t*)"\x00", 1);
  enc28j60_write_buffer(data, len);

  enc28j60_wcr16(ETXST, ENC28J60_TXSTART);
  enc28j60_wcr16(ETXND, ENC28J60_TXSTART + len);

  enc28j60_bfs(ECON1, ECON1_TXRTS); // Request packet send
}


uint16_t eth_recvpkt(uint8_t *buf, uint16_t buflen) {
  uint16_t len = 0, rxlen, status, temp;

  // Есть ли принятые пакеты?
  if (enc28j60_rcr(EPKTCNT)) {

    // Считываем заголовок:
    enc28j60_wcr16(ERDPT, enc28j60_rxrdpt);

    enc28j60_read_buffer((void*)&enc28j60_rxrdpt, sizeof(enc28j60_rxrdpt));
    enc28j60_read_buffer((void*)&rxlen, sizeof(rxlen));
    enc28j60_read_buffer((void*)&status, sizeof(status));

    // Пакет принят успешно?
    if (status & 0x80) { //success
      len = rxlen - 4; // Выбрасываем контрольную сумму
      // Читаем пакет в буфер (если буфера не хватает, пакет обрезается)
      if (len > buflen) len = buflen;
      enc28j60_read_buffer(buf, len);
    }

    // Устанавливаем ERXRDPT на адрес следующего пакета - 1 (минус 1 - из-за бага)
    temp = (enc28j60_rxrdpt - 1) & ENC28J60_BUFEND;
    enc28j60_wcr16(ERXRDPT, temp);

    // Decrement packet counter
    enc28j60_bfs(ECON2, ECON2_PKTDEC); // Сбрасывает флаг PKTIF ???
  }

  return len;
}


void eth_get_addr(uint8_t* addr) {
  addr[0]=enc28j60_rcr(MAADR5);
  addr[1]=enc28j60_rcr(MAADR4);
  addr[2]=enc28j60_rcr(MAADR3);
  addr[3]=enc28j60_rcr(MAADR2);
  addr[4]=enc28j60_rcr(MAADR1);
  addr[5]=enc28j60_rcr(MAADR0);
}
