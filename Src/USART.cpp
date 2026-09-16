#include <stm32f407xx.h>
#include "stm32f407xx_1.h"
#include "USART.h"
#include "string.h"

double uart_timeout=0,uart_timeout_o=0;

void USART_SetBaudRate(USART_TypeDef *pUSARTx, uint32_t BaudRate);
void USART_PeriClockControl(USART_TypeDef *pUSARTx, uint8_t EnorDi)
{
	if(EnorDi==ENABLE)
	{
		switch((int)pUSARTx)
		{
			case (int)USART1:
			USART1_PCLK_EN;
			break;
			
			case (int)USART2:
			USART2_PCLK_EN;	
			break;
			
			case (int)USART3:
			USART3_PCLK_EN;	
			break;
			
			case (int)UART4:
			UART4_PCLK_EN;	
			break;
			
			case (int)UART5:
			UART5_PCLK_EN;	
			break;
			
			case (int)USART6:
			USART6_PCLK_EN;	
			break;
		}
	}
	else
	{
	}
}

void USART_Init(USART_Handle_t *pUSARTHandle)
{
	  //Implement the code to enable the Clock for given USART peripheral
	 USART_PeriClockControl(pUSARTHandle->pUSARTx,ENABLE);
	 USART_PeripheralControl(pUSARTHandle->pUSARTx, ENABLE);
	
	
	//Implement the code to configure the Word length configuration item 
	pUSARTHandle->pUSARTx->CR1|= pUSARTHandle->USART_Config.USART_WordLength << 12 ;
	
	//Implement the code to configure the number of stop bits inserted during USART frame transmission 
	pUSARTHandle->pUSARTx->CR2 |= pUSARTHandle->USART_Config.USART_NoOfStopBits << 12;

	pUSARTHandle->pUSARTx->CR1|=(1<<15);
	//Baud rate 
	USART_SetBaudRate(pUSARTHandle->pUSARTx,pUSARTHandle->USART_Config.USART_Baud);
	
  //Configuration of parity control bit fields
	if ( pUSARTHandle->USART_Config.USART_ParityControl == PARITY_EVEN)
	{
		//Implement the code to enale the parity control 
		pUSARTHandle->pUSARTx->CR1 |=(1<<10);
  
	}else if (pUSARTHandle->USART_Config.USART_ParityControl == PARITY_ODD )
	{
	    pUSARTHandle->pUSARTx->CR1|=(1<<10);
      pUSARTHandle->pUSARTx->CR1|=(1<<9);

	}
	
	

	//Enable USART Tx and Rx engines according to the USART_Mode configuration item
	if ( pUSARTHandle->USART_Config.USART_Mode == _RX)
	{
		//Implement the code to enable the Receiver bit field 
		//pUSARTHandle->pUSARTx->CR1&=~(1<<3);
		pUSARTHandle->pUSARTx->CR1|= (1 << 2);
	}else if (pUSARTHandle->USART_Config.USART_Mode == _TX)
	{
		//Implement the code to enable the Transmitter bit field 
		//pUSARTHandle->pUSARTx->CR1=~(1<<2);
		pUSARTHandle->pUSARTx->CR1 |= (1<<3);

	}else if (pUSARTHandle->USART_Config.USART_Mode == _TXRX)
	{
		//Implement the code to enable the both Transmitter and Receiver bit fields 
		pUSARTHandle->pUSARTx->CR1 |= ((1<<2)|(1<<3));
	}

	//peri enable
	
}

void USART_SendData(USART_TypeDef *pUSARTHandle,uint8_t pTxBuffer)
{
      while(!(pUSARTHandle->SR>>7 & 0x01));
			pUSARTHandle->DR=pTxBuffer; 
			while(!(pUSARTHandle->SR>>6 & 0x01));
}

/*void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t pTxBuffer, uint32_t Len)
{

	uint16_t *pdata;
   //Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//Implement the code to wait until TXE flag is set in the SR
		 while(!(pUSARTHandle->pUSARTx->SR>>7 & 0x01));

         //Check the USART_WordLength item for 9BIT or 8BIT in a frame
		if(pUSARTHandle->USART_Config.USART_WordLength ==  USART_9BITS)
		{
			//if 9BIT, load the DR with 2bytes masking the bits other than first 9 bits 
			pdata = (uint16_t*) pTxBuffer;
			pUSARTHandle->pUSARTx->DR= (*pdata & (uint16_t)0x01FF);
			
			//check for USART_ParityControl
			if(pUSARTHandle->USART_Config.USART_ParityControl == PARITY_DISABLE)
			{
				//No parity is used in this transfer. so, 9bits of user data will be sent
				//Implement the code to increment pTxBuffer twice 
				pTxBuffer++;
				pTxBuffer++;
			}
			else
			{
				//Parity bit is used in this transfer . so , 8bits of user data will be sent
				//The 9th bit will be replaced by parity bit by the hardware
				pTxBuffer++;
			}
		}
		else
		{
			//This is 8bit data transfer 
			pUSARTHandle->pUSARTx->DR = (pTxBuffer  & (uint8_t)0xFF);
			
			//Implement the code to increment the buffer address
			//pTxBuffer++;
		}
	}

	//Implement the code to wait till TC flag is set in the SR
	while(!(pUSARTHandle->pUSARTx->SR>>6 & 0x01));
}
*/


void USART_SetBaudRate(USART_TypeDef *pUSARTx, uint32_t BaudRate)
{

	//Variable to hold the APB clock
	uint32_t PCLKx;

	uint32_t usartdiv;

	//variables to hold Mantissa and Fraction values
	uint32_t M_part,F_part;

  uint32_t tempreg=0;

  //Get the value of APB bus clock in to the variable PCLKx
  if(pUSARTx == USART1 || pUSARTx == USART6)
  {
	   //USART1 and USART6 are hanging on APB2 bus
	   PCLKx = 0x54;
  }else
  {
	   PCLKx = 0x2A;
  }

  //Check for OVER8 configuration bit
  if(pUSARTx->CR1&(1<<15))
  {
	   //OVER8 = 1 , over sampling by 8
	   usartdiv = (PCLKx*0x1E848/BaudRate);
  }else
  {
	   usartdiv = (0x501BD00/32*BaudRate);//84mz/2
  }

  //Calculate the Mantissa part

  M_part = usartdiv/100;

  //Place the Mantissa part in appropriate bit position . refer USART_BRR
  tempreg |= M_part << 4;

  //Extract the fraction part
   F_part = (usartdiv - (M_part * 100));

  //Calculate the final fractional
  if(pUSARTx->CR1 & (1<<15))
   {
	  //OVER8 = 1 , over sampling by 8
	  F_part =  (((F_part*8))/100);

   }else
   {
	   //over sampling by 16
	 //  F_part = (((F_part*16)+50)/(100&(uint8_t)0x0F));

   }

  //copy the value of tempreg in to BRR register
  pUSARTx->BRR = (M_part<<4)|(F_part);
}

void USART_PeripheralControl(USART_TypeDef *pUSARTx, uint8_t EnOrDi)
{
	if(ENABLE)
	{
		switch((int)pUSARTx)
		{
			case (int)UART4:
		  pUSARTx->CR1|=(1<<13);
			break;
			
			case (int)USART1:
		  pUSARTx->CR1|=(1<<13);
			break;
			
			case (int)USART2:
		  pUSARTx->CR1|=(1<<13);
			break;
			
			case (int)USART3:
		  pUSARTx->CR1|=(1<<13);
			break;

			case (int)USART6:
			pUSARTx->CR1|=(1<<13);
			break;	

			case (int)UART5:
		  pUSARTx->CR1|=(1<<13);
			break;
		}
	}
	else
		pUSARTx->CR1&=~(1<<13);
}
				
uint8_t USART_ReceiveData(USART_TypeDef *pUSARTx)
{
	uart_timeout=0;
	
	while(!((pUSARTx->SR>>5 & 0x01 || (uart_timeout > 15000))))//15000
	{
		uart_timeout++;
	} 
	return pUSARTx->DR;

}

//uint8_t USART_ReceiveData_From_Odrive(USART_TypeDef *pUSARTx)
//{
//	uart_timeout_o=0;
//	
//	while(!((pUSARTx->SR>>5 & 0x01 || (uart_timeout_o > 15000))))//15000
//	{
//		uart_timeout_o++;
//	} 
//	if(uart_timeout_o>15000)
//	{
//	return 0;
//	}
//	else
//	{
//	return pUSARTx->DR;
//	}
//}

	
uint8_t USART_receive(USART_TypeDef *pUSARTx)
{		
		//    /***************  CHECKING THE FLAG STATUS  ******************/
		//		  while(USART_getflagstatus(pUSARTx,USART_RXNE_FLAG) == FLAG_RESET);
	
				uint8_t data = pUSARTx->DR;
			
		//		/***************  CHECKING THE FLAG STATUS  ******************/
		//			while(USART_getflagstatus(pUSARTx,USART_TC_FLAG) == FLAG_RESET);		
			
		return data;
}
/*********************************************************************************************************************
***************************************** VESC SPECIFIC **************************************************************
*********************************************************************************************************************/

void USART_Send_uint8_string (USART_TypeDef *pUSARTHandle,uint8_t *Uart_string,int len)
{
	while (len!=0)
	{
		len--;
		while(!(pUSARTHandle->SR>>7 & 0x01)){
		}
			pUSARTHandle->DR = *Uart_string++; 
			while(!(pUSARTHandle->SR>>6 & 0x01));
	}
}
void USART_Send_string(USART_TypeDef *pUSARTHandle,char *pTxbuffer)
{
	for(int i=0;i<strlen(pTxbuffer);i++)
	{	
   	USART_SendData_char(pUSARTHandle,pTxbuffer[i]);
	}		
}

void USART_Send_array(USART_TypeDef *pUSARTHandle,char *pTxbuffer)
{
	for(int i=0;i<(sizeof(pTxbuffer));i++)
	{	
   	USART_SendData(pUSARTHandle,pTxbuffer[i]);
	}		
}



void USART_SendData_char(USART_TypeDef *pUSARTHandle,char pTxBuffer)
{
  while(!(pUSARTHandle->SR>>7 & 0x01));
	pUSARTHandle->DR =pTxBuffer; 
	while(!(pUSARTHandle->SR>>6 & 0x01));
}

