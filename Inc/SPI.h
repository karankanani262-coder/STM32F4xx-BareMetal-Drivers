#include <stm32f407xx.h>
#include <stdint.h>
#ifndef _SPI_H
#define _SPI_H

typedef struct
{
	uint8_t DeviceMode;                  //@MODE
	uint8_t BusConfig;                   //@BUS
	uint8_t Frequency;                   //@FREQ
	uint8_t DFF;                         //@DATA
	uint8_t CPOL;                        //@CPOL
	uint8_t CPHA;                        //@CPHA
	uint8_t SSM;                         //@SSM
	uint8_t FF;                          //@FF
}SPI_Config_t;  

typedef struct
{
	SPI_TypeDef *pSPI;
	SPI_Config_t SPIConfig;
}SPI_Handle_t;

//Communication Type of SPI(@BUS)
#define Half_Duplex 1
#define Full_Duplex 0

//SPI Mode of Operation(@MODE)
#define SPI_MASTER  1
#define SPI_SLAVE   0

//SPI Data Bits(@DATA)
#define SPI_8_BIT    0 
#define SPI_16_BIT   1

//SPI SSM BIT(@SSM)
#define SSM_EN 1
#define SSM_DI 0

//SPI FF(@FF)
#define LSB 1
#define MSB 0

//CPHA
#define CPHA_0 0
#define CPHA_1 1

//CPOL
#define CPOL_0 0
#define CPOL_1 1

//SPI Clock Frequency(@FREQ)
#define SPI_F_2     0
#define SPI_F_4     1
#define SPI_F_8     2
#define SPI_F_16    3
#define SPI_F_32    4
#define SPI_F_64    5
#define SPI_F_128   6
#define SPI_F_256   7

//SPI Clock Enable Macros
#define SPI1_CLK_EN              RCC->APB2ENR|=(1<<12)               
#define SPI2_CLK_EN              RCC->APB1ENR|=(1<<14)              
#define SPI3_CLK_EN              RCC->APB1ENR|=(1<<15)   

//SPI Enable and Diable 
#define SPI1_PER_EN                SPI1->CR1|=(1<<6)
#define SPI1_PER_DI                SPI1->CR1&=~(1<<6)

#define SPI2_PER_EN                SPI2->CR1|=(1<<6)
#define SPI2_PER_DI                SPI2->CR1&=~(1<<6)

#define SPI3_PER_EN                SPI3->CR1|=(1<<6)
#define SPI3_PER_DI                SPI3->CR1&=~(1<<6)

//SPI Interrupt types Macros
#define SPI_TX_INT          7
#define SPI_RX_INT          6
#define SPI_ERROR_INT       5


//SPI Peripheral Clock Function
void SPI_ClockControl(SPI_TypeDef *pSPIx,uint8_t EnorDi);
//SPI Init and DeInit Functions
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_TypeDef *pSPIx);
//SPI Data Send and Receive
uint8_t SPI_Communicate(SPI_TypeDef *pSPIx,char pTxBuffer);
//SPI IRQ Functions
void SPI_IRQconfig(SPI_TypeDef *pSPIx,uint8_t IRQNumber,uint8_t inttype,uint8_t EnorDi);
void SPI_IRQHandling(SPI_Handle_t *pHandle);
void SPI_IRQPriority(uint32_t IRQprio,uint8_t IRQNum);


#endif
