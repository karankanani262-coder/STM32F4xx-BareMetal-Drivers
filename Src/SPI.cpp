#include <stdint.h>
#include <stm32f407xx.h>
#include "stm32f407xx_1.h"
#include "SPI.h"
void SPI_ClockControl(SPI_TypeDef *pSPIx,uint8_t EnorDi)
{
if(EnorDi==ENABLE)
	{
		
		switch((int)pSPIx)
		{
			case (int)SPI1:
			SPI1_CLK_EN;
			
			break;
			
			case (int)SPI2:
			SPI2_CLK_EN;	
			
			break;
			
			case (int)SPI3:
			SPI3_CLK_EN;
      	
			break;
		}
	}
	else
	{
		switch((int)pSPIx)
		{
			case (int)SPI1:
			//GPIOA_PCLK_DI;
			break;
			
			case (int)SPI2:
			//GPIOB_PCLK_DI;	
			break;
			
			case (int)SPI3:
			//GPIOC_PCLK_DI;	
			break;
		}
  }
}

void SPI_Init(SPI_Handle_t *pSPIHandle)
{ 
	 pSPIHandle->pSPI->CR1|=pSPIHandle->SPIConfig.Frequency<<3;
	 pSPIHandle->pSPI->CR1|=pSPIHandle->SPIConfig.CPOL<<1;
	 pSPIHandle->pSPI->CR1|=pSPIHandle->SPIConfig.CPHA<<0;
   pSPIHandle->pSPI->CR1|=pSPIHandle->SPIConfig.DFF<<11;
	 pSPIHandle->pSPI->CR1|=pSPIHandle->SPIConfig.FF<<7;
	 pSPIHandle->pSPI->CR1|=(1<<9);
	 pSPIHandle->pSPI->CR1|=(1<<8);
	 if(pSPIHandle->SPIConfig.DeviceMode==1)
	  pSPIHandle->pSPI->CR1|=(1<<8)|(1<<9);
	 pSPIHandle->pSPI->CR1|=pSPIHandle->SPIConfig.BusConfig<<15;
	 pSPIHandle->pSPI->CR1|=pSPIHandle->SPIConfig.DeviceMode<<2;
	 pSPIHandle->pSPI->CR1|=(1<<6);
	
}

uint8_t SPI_Communicate(SPI_TypeDef *pSPIx,char pTxBuffer)
{
     pSPIx->DR=pTxBuffer;
	   while(!(pSPIx->SR&0x01));
	   return pSPIx->DR;
}

void SPI_IRQconfig(SPI_TypeDef *pSPIx,uint8_t IRQNumber,uint8_t inttype,uint8_t EnorDi)
{
	 if(EnorDi== ENABLE)
		{
			   if(IRQNumber<=31)
		      *NVIC_ISER0|=(1<<IRQNumber);
				 else if(IRQNumber >31 && IRQNumber<64)
				  *NVIC_ISER1|=(1<<(IRQNumber-32));
         else if(IRQNumber>=64 && IRQNumber<96)
				  *NVIC_ISER3|=(1<<(IRQNumber-64));
		}
		else
		{
			   if(IRQNumber<=31)
				  *NVIC_ICER0|=(1<<6);
				 else if(IRQNumber >31 && IRQNumber<64)
				  *NVIC_ICER1|=(1<<(IRQNumber-32));
         else if(IRQNumber>=64 && IRQNumber<96)
				  *NVIC_ICER2|=(1<<(IRQNumber-64));
		}
    pSPIx->CR2|=(1<<inttype);
}
void SPI_IRQHandling(SPI_Handle_t *pHandle)
{
	
}
/*
	uint8_t SPI_DATA_BUFFER;
	SPI_DATA_BUFFER=49;
	uint32_t *SPI_DUMMY;

	uint8_t SPI_READ_DUMMY;
  (void)SPI_READ_DUMMY;
	uint8_t SPI_DATA;
  bool x;
	SPI_GPIOConfig();
	SPI_ClockControl(SPI2,ENABLE);

	SPI_Handle.pSPI=SPI2;
	
	SPI_Handle.SPIConfig.CPHA=CPHA_0;
	SPI_Handle.SPIConfig.CPOL=CPOL_0;
	SPI_Handle.SPIConfig.SSM=SSM_EN;
	SPI_Handle.SPIConfig.FF=MSB;
  SPI_Handle.SPIConfig.DeviceMode=SPI_MASTER;
	SPI_Handle.SPIConfig.BusConfig=Full_Duplex;
	SPI_Handle.SPIConfig.DFF=SPI_8_BIT;
	SPI_Handle.SPIConfig.Frequency=SPI_F_8;
  SPI_Init(&SPI_Handle);
*/
