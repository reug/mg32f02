#ifndef MG32F02_DMA_H
#define MG32F02_DMA_H

#include <stdint.h>


/// Source from MG32x02z_DMA_DRV.h
enum DMA_SourcenRequestDef {
    DMA_MEM_Read=0,     /*!< from memory */
    DMA_ADC0_IN=1,      /*!< from ADC conversion data */
    DMA_I2C0_RX=2,      /*!< from I2C0 external in data */
    DMA_I2C1_RX=3,      /*!< from I2C1 external in data */
    DMA_URT0_RX=4,      /*!< from URT0 RX data */
    DMA_URT1_RX=5,      /*!< from URT1 RX data */
    DMA_URT2_RX=6,      /*!< from URT2 RX data */
    DMA_SPI0_RX=8,      /*!< from SPI0 MISO data */
    DMA_USB_RX=10,      /*!< from USB_RX data */
    DMA_TM36_IC3=15,    /*!< from TM36 capture data */
};


/// Source from MG32x02z_DMA_DRV.h
enum DMA_DestinationRequestDef {
    DMA_MEM_Write=0,    /*!< to memory */
    DMA_DAC0_OUT=1,     /*!< to DAC output control */
    DMA_I2C0_TX=2,      /*!< to I2C0 output */
    DMA_I2C1_TX=3,      /*!< to I2C1 output */
    DMA_URT0_TX=4,      /*!< to URT0 TX output */
    DMA_URT1_TX=5,      /*!< to URT1 TX output */
    DMA_URT2_TX=6,      /*!< to URT2 TX output */
    DMA_SPI0_TX=8,      /*!< to SPI0 MOSI output */
    DMA_USB_TX=10,      /*!< to USB TX output */
    DMA_GPL_Write=11,   /*!< to GPL function control */
    DMA_TM36_CC0B=12,   /*!< to TM36 channel 0 - buffer data */
    DMA_TM36_CC1B=13,   /*!< to TM36 channel 1 - buffer data */
    DMA_TM36_CC2B=14,   /*!< to TM36 channel 2 - buffer data */
};

void dma_init();

/// Установка источника и приемника
void dma_setup_sd(uint8_t ch, uint16_t srcdet);

/// Установка адреса источника в памяти
void dma_setup_memsrc(uint8_t ch, void* addr);

/// Установка адреса приемника в памяти
void dma_setup_memdst(uint8_t ch, void* addr);

/// Включение прерывания INT_DMA по флагам, указанным в flags согласно формату DMA_CHnA (b2)
void dma_setup_int(uint8_t ch, uint8_t flags);


#endif // MG32F02_DMA_H
