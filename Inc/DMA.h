#include <stm32f407xx.h>
#include "string.h"
#include "TIMER.h"
#ifndef DMA__H
#define DMA__H

//DMA Enable Clock Macros
#define DMA1_PCLK_EN      RCC->AHB1ENR|=(1<<21)
#define DMA2_PCLK_EN      RCC->AHB1ENR|=(1<<22)
extern uint32_t DATA[16];
typedef struct
{
	uint8_t channel;
	uint8_t data_width;
	uint8_t direction;
	uint8_t mode;
	uint8_t fifo_threshold;
	uint8_t circular_mode;
	uint8_t transfer_mode;
	uint8_t Stream_priority;
	uint32_t Source_addr; // set in start api 
	uint32_t desti_addr;
	bool Interrupt_Enordi;
}DMA_Init;


typedef struct
{
  DMA_TypeDef *pDMAx;
	DMA_Stream_TypeDef *pstream;
	DMA_Init DMA_Config;
}DMA_Handle_t;

typedef struct {
    DMA_Stream_TypeDef *tx_stream;
    DMA_Stream_TypeDef *rx_stream;
} Usart_DMA_Stream_Map;


typedef struct {
    DMA_Stream_TypeDef *tx_stream;
    DMA_Stream_TypeDef *rx_stream;
} I2C_DMA_Stream_Map;



//  mappings for each USART peripheral
const Usart_DMA_Stream_Map usart_dma_map[] = {
     {DMA2_Stream7, DMA2_Stream2},  // USART1 TX and RX stream
     {DMA1_Stream6, DMA1_Stream5},  // USART2 TX and RX stream
     {DMA1_Stream3, DMA1_Stream1},  // USART3 TX and RX stream
     {DMA1_Stream4, DMA1_Stream2},  // USART4 TX and RX stream
     {DMA1_Stream7, DMA1_Stream0},  // USART5 TX and RX stream
     {DMA2_Stream6, DMA2_Stream2},  // USART6 TX and RX stream
};
//  mappings for each I2C peripheral
const I2C_DMA_Stream_Map i2c_dma_map[] = {
     {DMA1_Stream6, DMA1_Stream0},  // i2c1 TX and RX stream
     {DMA1_Stream7, DMA1_Stream2},  // i2c2 TX and RX stream
     {DMA1_Stream4, DMA1_Stream2},  // i2c3 TX and RX stream
};


/*--------------------------@Streams--------------------------------
select the stream of the peripheral for which request is to be sent. 
------------------------------------------------------------------*/
//@USART Peripheral
#define usart1_tx_stream   DMA2_Stream7
#define usart1_rx_stream   DMA2_Stream2
#define usart2_tx_stream   DMA1_Stream6
#define usart2_rx_stream   DMA1_Stream5
#define usart3_tx_stream   DMA1_Stream3
#define usart3_rx_stream   DMA1_Stream1
#define usart4_tx_stream   DMA1_Stream4
#define usart4_rx_stream   DMA1_Stream2
#define usart5_tx_stream   DMA1_Stream7
#define usart5_rx_stream   DMA1_Stream0
#define usart6_tx_stream   DMA2_Stream6
#define usart6_rx_stream   DMA2_Stream2
/*
Not available on board.. 
#define usart7_tx_stream   DMA_Stream_0
#define usart7_rx_stream   DMA_Stream_0
#define usart8_tx_stream   DMA_Stream_0
#define usart8_rx_stream   DMA_Stream_0
*/

//@ADC Peripheral
#define adc1_stream   	 DMA2_Stream0
#define adc2_stream   	 DMA2_Stream2


#define I2C1_tx_stream   DMA1_Stream6
#define I2C1_rx_stream   DMA1_Stream0
#define I2C2_tx_stream   DMA1_Stream7
#define I2C2_rx_stream   DMA1_Stream2
#define I2C3_tx_stream   DMA1_Stream4
#define I2C3_rx_stream   DMA1_Stream2



/*--------------------------@channel--------------------------------
select the channel of the peripheral for which request is to be sent. 
------------------------------------------------------------------*/
//@USART Peripheral
#define usart1_tx_channel   4
#define usart1_rx_channel   4
#define usart2_tx_channel   4
#define usart2_rx_channel   4
#define usart3_tx_channel   4
#define usart3_rx_channel   4
#define usart4_tx_channel   4
#define usart4_rx_channel   4
#define usart5_tx_channel   4
#define usart5_rx_channel   4
#define usart6_tx_channel   5
#define usart6_rx_channel   5
#define usart7_tx_channel   0
#define usart7_rx_channel   0
#define usart8_tx_channel   0
#define usart8_rx_channel   0

//@ADC Peripheral
#define adc1_channel   			0
#define adc2_channel   			1

#define I2C1_tx_channel   	1
#define I2C1_rx_channel   	1
#define I2C2_tx_channel   	7
#define I2C2_rx_channel   	7
#define I2C3_tx_channel   	3
#define I2C3_rx_channel   	3



/*--------------------------@direction--------------------------------
transfer is from m2m m2p or p2m. 
--------------------------------------------------------------------*/
#define m2m	2	
#define m2p	1 
#define p2m	0

/*--------------------------@data_width-------------------------------
Memory size and peripheral size for data transfer.
--------------------------------------------------------------------*/
#define byte_to_byte					0					//08 bits 
#define halfword_to_halfword  1					//16 bits 
#define word_to_word  				2				//32 bits 


/*--------------------------@mode--------------------------------
transfer is through direct or fifo mode. 
---------------------------------------------------------------*/
#define direct_mode			0
#define fifo_mode 			1


/*--------------------------@threshold--------------------------------
-->only if fifo mode is selected.
--------------------------------------------------------------------*/
#define mul_1_4	  0
#define mul_1_2		1
#define mul_3_4		2
#define mul_1			3


/*--------------------------@transfer_mode--------------------------*/
#define single_tranfer	0
#define burst_tranfer 	1

/*--------------------------@stream_priority--------------------------
-->set only if needed.
--------------------------------------------------------------------*/
#define low					0
#define medium 			1
#define high 				2
#define Very high 	3

/*--------------------------@Interrupt_Enordi--------------------------
Interrupt Enable macros 
--------------------------------------------------------------------*/
#define Enable 1
#define Disable 0


/*--------------------------@Interrupts------------------------*/

void DMA_PeriClockControl(DMA_TypeDef *pDMAx, uint8_t EnorDi);
void dma_Init(DMA_Handle_t *pdmahandle);
DMA_Stream_TypeDef* Get_usart_DMA_Stream(USART_TypeDef *pUsart, uint8_t is_tx);

/*----------------------------- USART -------------------------------*/

void Dma_Usart_Send(USART_TypeDef *pUsart,uint32_t *Source_addr);
void Dma_Usart_Send_array(USART_TypeDef *pUsart,uint32_t *Source_addr,uint8_t size);
void Dma_Usart_Receive(USART_TypeDef *pUsart, uint8_t *dataBuffer, uint32_t dataLength);

 
/*----------------------------- ADC -------------------------------*/
void dma_adc(ADC_TypeDef *pADCx,uint16_t *dataBuffer,uint8_t dataLength);
 DMA_Stream_TypeDef* Get_adc_DMA_Stream(ADC_TypeDef *pADCx);
 void Start_ADC_DMA(uint16_t *buffer, uint8_t length);
 
 
void Dma_I2C_Send(I2C_TypeDef *pI2c, uint8_t *dataBuffer, uint32_t dataLength, uint8_t slaveAddress);
void Dma_I2C_Receive(I2C_TypeDef *pI2c, uint8_t *dataBuffer, uint32_t dataLength, uint8_t slaveAddress);
#endif
