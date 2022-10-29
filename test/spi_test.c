#include "spi_test.h"
#include "core.h"
#include "MG32x02z_GPIO.h"
#include "MG32x02z_SPI.h"
#include "MG32x02z_EXIC.h"
#include "MG32x02z_DMA.h"
#include "hwcf.h"
#include "ulib.h"
#include "utils.h"
#include "ic/enc28j60.h"
#include "ethernet.h"
#include "dma.h"


/// Локальный MAC адрес
const uint8_t addr[6]={0x02,0xEE,0x10,0x00,0x00,0x01};

/// Целевой MAC адрес
const uint8_t dest[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

/// Общие флаги событий:
volatile uint32_t state;

#define ST_PKTRECV 0x00000001
#define ST_PKTSENT 0x00000002





// **** Имитация узла IPv4 и простейшая реализация ARP протокола ****

/// Локальный IPv4 адрес 192.168.0.177
const uint32_t ipaddr=0xC0A800B1;

/// Возвращает IPv4 адрес из входящего кадра ARP запроса.
/// Если кадр не содержит ARP-запрос, возвращает 0.
uint32_t arp_get_ipaddr(const uint8_t* pkt) {
  if (
      (*(uint16_t*) (pkt+12) == 0x0608) // проверяем код протокола 0x0806 (big endian)
      && (*(uint16_t*) (pkt+20) == 0x0100)) // проверяем код операции (1-запрос, 2-ответ) (big endian)
  {
    return __REV(*(uint32_t*)(pkt+38)); // возвращаем адрес в формате little endian
  }
  return 0;
}

/// Формирует кадр с ARP-ответом.
void arp_gen_reply(uint8_t* pkt) {
  memcpy(eth_frame+32,eth_frame+6,6);   // target MAC
  memcpy(eth_frame+38,eth_frame+28,4);  // target IP
  memcpy(eth_frame,eth_frame+6,6);      // Dest ADDR
  eth_setframe_addr(addr);              // Src ADDR
  eth_frame[21]=2;                      // reply opcode
  memcpy(eth_frame+22,addr,6);          // < sender MAC
  *(uint32_t*)(eth_frame+28)=__REV(ipaddr); // < sender IP
}

// **** **** **** ****


void pin_test(uint8_t p) {
  RH(PA_CR8_h0) = 2;  RH(PA_CR9_h0) = 2;  RH(PA_CR10_h0) = 2;  RH(PA_CR11_h0) = 2;
  while (1) {
    RH(PA_SC_h0) = (1 << p);  delay_ms(1);
    RH(PA_SC_h1) = (1 << p);  delay_ms(1);
  }
}


/// Обработчик внешнего прерывания INT_SPI0 без DMA.
void exint0_hdl_nodma() {
  led1_on();
  enc28j60_bfc(EIE,EIE_INTIE); // сбрасываем INTIE бит согласно даташиту

  eth_frame_len=eth_recvpkt(eth_frame,ETH_FRAME_MAXSIZE);
  if (eth_frame_len) {
    debug('L',eth_frame_len);
    debugbuf(eth_frame,14);
  }

  enc28j60_bfs(EIE,EIE_INTIE); // устанавливаем INTIE бит согласно даташиту
  RH(EXIC_PA_PF_h0) = HW_EXINT0_MASK; // сбрасываем флаг (EXIC_PA_PF_PA12_PF_mask_h0)
  led1_off();
}


/// Обработчик внешнего прерывания INT_SPI0.
/// Инициализация процедуры считывания принятого пакета.
void exint0_hdl_dma() {
  uint16_t rxlen, status;
  //led1_on();

  enc28j60_bfc(EIE,EIE_INTIE); // сбрасываем INTIE бит согласно даташиту

  // Считываем заголовок:
  enc28j60_wcr16(ERDPT, enc28j60_rxrdpt); // Устанавливаем начальный адрес в буфере для считывания
  enc28j60_read_buffer((void*)&enc28j60_rxrdpt, sizeof(enc28j60_rxrdpt));
  enc28j60_read_buffer((void*)&rxlen, sizeof(rxlen));
  enc28j60_read_buffer((void*)&status, sizeof(status));

  // Пакет принят успешно?
  if (status & 0x0080) { //success
    // Читаем пакет в буфер (если буфера не хватает, пакет обрезается)
    eth_frame_len = (rxlen > ETH_FRAME_MAXSIZE) ? ETH_FRAME_MAXSIZE : rxlen;
    enc28j60_select();
    enc28j60_tx(ENC28J60_SPI_RBM);
    // Далее необходимо принять из SPI eth_frame_len байт
    spi_dma_start_rx(eth_frame_len); // запускаем прием данных кадра через DMA
  }

  // Функции enc28j60_* далее применять нельзя до завершения операции DMA !!!
  RH(EXIC_PA_PF_h0) = HW_EXINT0_MASK; // сбрасываем флаг (EXIC_PA_PF_PA12_PF_mask_h0)
  //led1_off();
}


/// Настройка внешнего прерывания EXIC
void exint_setup() {
  // Настройка вывода и контроллера EXIC
  RH(HW_EXINT0_CRH0) =
    //PX_CR_INV_enable_h0 | // инверсия не нужна, активный уровень низкий по-умолчанию!
    PX_CR_PU_enable_h0 |  // подтяжка
    PX_CR_IOM_din_h0;     // цифровой вход
  //RW(EXIC_PA_TRGS_w) |= (1 << (HW_EXINT0_BIT*2)); // Level
  RW(EXIC_PA_TRGS_w) |= (2 << (HW_EXINT0_BIT*2)); // Edge
  RW(EXIC_PA_MSK_w) |= (1 << HW_EXINT0_BIT); // OR (AOM
  RB(EXIC_INT_b0) |= EXIC_INT_PA_IEA_enable_b0;
  // Установка обработчика прерывания:
  SVC2(SVC_HANDLER_SET,3,exint0_hdl_nodma);
  //SVC2(SVC_HANDLER_SET,3,exint0_hdl_dma);
  RW(CPU_ISER_w) = (1 << 3); // SETENA 3
}


/// Обработчик прерывания DMA.
void dma_hdl() {
  uint32_t f;
  //led1_on();

  f = RW(DMA_STA_w);
  if (f & DMA_STA_CH0_TCF_happened_w) {
    // Завершен прием данных по SPI
    //RB(DMA_CH0A_b0) &= ~DMA_CH0A_CH0_EN_enable_w; // reset DMA channel
    enc28j60_release();
    // Устанавливаем ERXRDPT на адрес следующего пакета - 1 (минус 1 - из-за бага)
    enc28j60_wcr16(ERXRDPT, (enc28j60_rxrdpt - 1) & ENC28J60_BUFEND);

    // Decrement packet counter

    // Datasheet: After decrementing, if EPKTCNT is ‘0’, the EIR.PKTIF flag will automatically be cleared.
    // Otherwise, it will remain set, indicating that additional packets are in the receive buffer
    // and are waiting to be processed.
    // Attempts to decrement EPKTCNT below 0 are ignored.
    enc28j60_bfs(ECON2, ECON2_PKTDEC); // Сбрасывает флаг PKTIF, если счетчик ==0

    RB(DMA_CH0A_b3) |= DMA_CH0A_CH0_TC2F_mask_b3; // clear flag

    // Разрешаем следующее прерывание ENC28J60:
    enc28j60_bfs(EIE,EIE_INTIE); // устанавливаем INTIE бит согласно даташиту

    __disable_irq(); // опционально: запрещаем все прерывания МК до прочтения флагов state
    state |= ST_PKTRECV;
  }

  if (f & DMA_STA_CH1_TCF_happened_w) {
    // Завершена отправка данных по SPI

    while (!(RB(SPI0_STA_b0) & SPI_STA_TCF_happened_b0)); // дожидаемся фактического завершения передачи

    enc28j60_release();

    RB(DMA_CH1A_b3) |= DMA_CH0A_CH0_TC2F_mask_b3; // clear flag
    //RB(DMA_CH1A_b0) = 0;

    __disable_irq(); // опционально: запрещаем все прерывания МК до прочтения флагов state
    state |= ST_PKTSENT;
  }

  //led1_off();

}


/// Настройки DMA
void spi_dma_setup() {
  dma_init();

  // Настройка канала 0 для приема
  dma_setup(0,0);
  dma_setup_sd(0,
    DMA_SPI0_RX | // источник
    (DMA_MEM_Write << 8) // приемник
  );
  dma_setup_memdst(0,eth_frame);

  // Настройка канала 1 для отправки
  dma_setup(1,0);
  dma_setup_sd(1,
    DMA_MEM_Read | // источник
    (DMA_SPI0_TX << 8) // приемник
  );
  dma_setup_memsrc(1,eth_frame);

  //RB(SPI0_CR0_b3) |= SPI_CR0_DMA_TXEN_enable_b3 | SPI_CR0_DMA_RXEN_enable_b3; // здесь нельзя это включать, т.к. сразу заблокируются флаги
  // SPI_CR0_DMA_MDS_enable_b3
  //dma_setup_int(0,DMA_CH0A_CH0_CIE_enable_b2); // включаем прерывание по завершению передачи
  RB(DMA_CH0A_b2) = DMA_CH0A_CH0_CIE_enable_b2;
  RB(DMA_CH1A_b2) = DMA_CH1A_CH1_CIE_enable_b2;

  RB(DMA_INT_b0) = DMA_INT_IEA_enable_b0;
  SVC2(SVC_HANDLER_SET,8,dma_hdl);
  RW(CPU_ISER_w) = (1 << 8); // SETENA 8
}


/// Запуск процедуры приема данных на канале 0, n - число байт
void spi_dma_start_rx(uint32_t n) {
  //RB(DMA_CH0A_b0) |= DMA_CH0A_CH0_EN_enable_w;
  RB(SPI0_CR0_b3) |= SPI_CR0_DMA_RXEN_enable_b3;
  dma_setup_amount(0,n);
  //dma_setup_amount(0,2);
  dma_start(0,DMA_CH0A_CH0_BSIZE_one_b1);
}


/// Запуск процедуры отправки данных на канале 1, n - число байт
void spi_dma_start_tx(uint32_t n) {
  uint8_t cbyte=0; // управляющий байт
led1_on();
  while(enc28j60_rcr(ECON1) & ECON1_TXRTS) {
    // TXRTS may not clear - ENC28J60 bug. We must reset
    // transmit logic in cause of Tx error
    if(enc28j60_rcr(EIR) & EIR_TXERIF) {
      enc28j60_bfs(ECON1, ECON1_TXRST);
      enc28j60_bfc(ECON1, ECON1_TXRST);
    }
  }

  enc28j60_wcr16(EWRPT, ENC28J60_TXSTART);
  enc28j60_write_buffer(&cbyte, 1);

  enc28j60_select();
  enc28j60_tx(ENC28J60_SPI_WBM);

  eth_frame_len=n;
  RB(SPI0_CR0_b3) |= SPI_CR0_DMA_TXEN_enable_b3;
  dma_setup_amount(1,n);
  dma_start(1,DMA_CH0A_CH0_BSIZE_one_b1);
}


void debug1() {
  uint8_t i;
  for (i=0x20; i<=0x3c; i+=4) {
    debug32hex(i,RW(DMA_Base+i));
  }
}


/// Буфер данных
#define BUFLEN 1536
uint8_t  buf[BUFLEN]; ///< данные буфера
uint32_t bufp;        ///< указатель буфера
uint32_t bufn;        ///< размер данных на отправку


void spi_test_master() {
  //uint16_t n;
  //uint32_t d;
  char s[8];
  uint32_t a; // IP address

  state=0;
  //pin_test(8);
  // Настройка выводов:
  HW_SPI0_SETMISO;  HW_SPI0_SETMOSI;  HW_SPI0_SETSCK;  HW_SPI0_SETNSS;

  // Инициализация, настройка тактирования:
  spi_init();

  // Настройка режима работы
  spi_setup_mode(
    //SPI_NSS_PEN |
    SPI_NSSO_INV | SPI_NSS_SWEN | // software NSS control
    SPI_NSSO_EN | SPI_MASTER | SPI_MSB | SPI_CPHA_LE | SPI_CPOL_LOW
    | SPI_CR0_DOUT_MDS_enable_w // надо включить, если нет резистора подтяжки
  );

  RB(SPI0_CR2_b1) = 1; // SPI0_RX_TH = 1
  RW(SPI0_CR2_w) |= (8 << SPI_CR2_DSIZE_shift_w); // Размер кадра в битах
  spi_flush_rx();

  eth_init(addr);
  uart_puts(PORT,"EREVID: ",UART_NEWLINE_NONE); strUint16hex(s,enc28j60_rcr(EREVID)); uart_puts(PORT,s,UART_NEWLINE_CRLF);
  uart_puts(PORT,"PHID1:  ",UART_NEWLINE_NONE); strUint16hex(s,enc28j60_rcr(PHID1)); uart_puts(PORT,s,UART_NEWLINE_CRLF);
  uart_puts(PORT,"PHID2:  ",UART_NEWLINE_NONE); strUint16hex(s,enc28j60_rcr(PHID2)); uart_puts(PORT,s,UART_NEWLINE_CRLF);
  uart_puts(PORT,"ERXFCON:",UART_NEWLINE_NONE); strUint16hex(s,enc28j60_rcr(ERXFCON)); uart_puts(PORT,s,UART_NEWLINE_CRLF);
  uart_puts(PORT,"MAC: ",UART_NEWLINE_NONE); eth_get_addr(s); debugbuf(s,6);

  enc28j60_release();

  exint_setup();
  eth_setup_int();

  while (1);

  // Настройка всех каналов DMA ( 0 - прием, 1 - отправка)
  spi_dma_setup();

  while (1) {
    if (state & ST_PKTRECV) {
      // Принят новый кадр
      eth_frame_len-=4;
      debug('R',eth_frame_len);
      //debugbuf(eth_frame,eth_frame_len);
      debugbuf(eth_frame,14);
      //debug32hex('C',*(uint32_t*)(eth_frame+eth_frame_len)); // CRC32 - little endian!
      debug32hex('S',crc32_block8(eth_frame,eth_frame_len));


//      if (a=arp_get_ipaddr(eth_frame)) {
//        debug32hex('A',a);
//        if (a==ipaddr) {
//          uart_puts(PORT,"> ARP_REPLY",UART_NEWLINE_CRLF);
//          arp_gen_reply(eth_frame);
//          spi_dma_start_tx(42);
//
//        }
//      }
      state &= ~ST_PKTRECV;
      __enable_irq();
    }

    if (state & ST_PKTSENT) {
      uart_puts(PORT,"T PKTSENT",UART_NEWLINE_CRLF);

      // Возвращаем работу флагов SPI:
      // Это не помогает:
      //RB(SPI0_CR0_b3) = 0; // возвращаем работу флагов SPI (отключаем DMA_TXEN и DMA_RXEN)
      // Вот это помогает:
      RB(SPI0_CR0_b0) &= 0xFE; // reset SPI0
      RB(SPI0_CR0_b0) |= 1; // reset SPI0
      // TODO
      // Как-то иначе пока не удалось восстановить работу флагов SPI
      // после завершения операции DMA (по отправке данных в SPI).


      // Буфер в ENC28J60 записан, теперь даем команду на отправку кадра
      // Устанавливаем границы кадра на отправку в буфере ENC28J60:
      enc28j60_wcr16(ETXST, ENC28J60_TXSTART);
      enc28j60_wcr16(ETXND, ENC28J60_TXSTART + eth_frame_len);
      enc28j60_bfs(ECON1, ECON1_TXRTS); // Даем саму команду
  led1_off();
      state &= ~ST_PKTSENT;
      __enable_irq();
    }
    //delay_ms(100);
  }
}

