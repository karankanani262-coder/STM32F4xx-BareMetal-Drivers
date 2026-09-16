#include "DMA.h"
uint32_t DATA[16];
extern IRQn_Type DMA1_IRQN[] = {
    DMA1_Stream0_IRQn, // Index 0 for Stream 0
    DMA1_Stream1_IRQn, // Index 1 for Stream 1
    DMA1_Stream2_IRQn, // Index 2 for Stream 2
    DMA1_Stream3_IRQn, // Index 3 for Stream 3
    DMA1_Stream4_IRQn, // Index 4 for Stream 4
    DMA1_Stream5_IRQn, // Index 5 for Stream 5
    DMA1_Stream6_IRQn, // Index 6 for Stream 6
    DMA1_Stream7_IRQn  // Index 7 for Stream 7	
};

extern IRQn_Type DMA2_IRQN[] = {
    DMA2_Stream0_IRQn, // Index 0 for Stream 0
    DMA2_Stream1_IRQn, // Index 1 for Stream 1
    DMA2_Stream2_IRQn, // Index 2 for Stream 2
    DMA2_Stream3_IRQn, // Index 3 for Stream 3
    DMA2_Stream4_IRQn, // Index 4 for Stream 4
    DMA2_Stream5_IRQn, // Index 5 for Stream 5
    DMA2_Stream6_IRQn, // Index 6 for Stream 6
    DMA2_Stream7_IRQn  // Index 7 for Stream 7	
};

volatile uint8_t km[10];
volatile uint8_t TOF_dist_3[16];
void DMA_PeriClockControl(DMA_TypeDef *pDMAx, uint8_t EnorDi)
{
	if(EnorDi==1)
	{
		switch((int)pDMAx)
		{
			case (int)DMA1:
			DMA1_PCLK_EN;
			break;
			
			case (int)DMA2:
			DMA2_PCLK_EN;	
			break;
		}
	}
}

void dma_Init(DMA_Handle_t *pdmahandle)
{
    DMA_PeriClockControl(pdmahandle->pDMAx, 1);
    pdmahandle->pstream->CR &= ~DMA_SxCR_EN;
    while (pdmahandle->pstream->CR & DMA_SxCR_EN); 
    //DMA2->LIFCR = (DMA_LIFCR_CTCIF2 | DMA_LIFCR_CTEIF2 | DMA_LIFCR_CDMEIF2 | DMA_LIFCR_CFEIF2);
    pdmahandle->pstream->CR &= ~(0x7 << 25);
    pdmahandle->pstream->CR |= (pdmahandle->DMA_Config.channel << 25);
    pdmahandle->pstream->CR &= ~(0x3 << 6);
    pdmahandle->pstream->CR |= (pdmahandle->DMA_Config.direction << 6);
    pdmahandle->pstream->CR &= ~(0x3 << 13);
    pdmahandle->pstream->CR |= (pdmahandle->DMA_Config.data_width << 13);
    pdmahandle->pstream->CR &= ~(0x3 << 11);  // Clear peripheral size bits
    pdmahandle->pstream->CR |= (pdmahandle->DMA_Config.data_width << 11);
    pdmahandle->pstream->CR |= DMA_SxCR_CIRC;
    pdmahandle->pstream->CR |= DMA_SxCR_MINC;
    pdmahandle->pstream->CR &= ~(0x3 << 16);
    pdmahandle->pstream->CR |= (pdmahandle->DMA_Config.Stream_priority << 16);
    pdmahandle->pstream->CR |= DMA_SxCR_EN;
}

/*------------------USART---------------------*/
void Dma_Usart_Receive(USART_TypeDef *pUsart, uint8_t *dataBuffer, uint32_t dataLength)
{
    DMA_Stream_TypeDef *pstream = Get_usart_DMA_Stream(pUsart, 0);  // 0 for RX stream
    pUsart->CR3 |= USART_CR3_DMAR;
	  pstream->CR &= ~DMA_SxCR_EN;
		while (pstream->CR & DMA_SxCR_EN);
    if ((uint32_t)pstream < (uint32_t)DMA2_Stream0)  // Check if it's DMA1
		{
    DMA1->LIFCR = (DMA_LIFCR_CTCIF0 | DMA_LIFCR_CTEIF0 | DMA_LIFCR_CDMEIF0 | DMA_LIFCR_CFEIF0);
		}
		else
		{
    DMA2->LIFCR = (DMA_LIFCR_CTCIF2 | DMA_LIFCR_CTEIF2 | DMA_LIFCR_CDMEIF2 | DMA_LIFCR_CFEIF2);
		}

    pstream->CR &= ~(DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE);           
    pstream->CR |= DMA_SxCR_MINC;          
    pstream->CR &= ~DMA_SxCR_PINC;          
    pstream->CR |= (2 << DMA_SxCR_PL_Pos);  
		pstream->NDTR = dataLength;
    pstream->PAR = (uint32_t)&pUsart->DR;   
    pstream->M0AR = (uint32_t)dataBuffer;
		pstream->CR |= DMA_SxCR_CIRC; 
    pstream->CR |= DMA_SxCR_EN;
}

int get_DMA_Stream(DMA_Handle_t *pdmahandle)
{
    DMA_Stream_TypeDef *pstream = pdmahandle->pstream;
    DMA_TypeDef *pDMAx = pdmahandle->pDMAx;

    if (pDMAx == DMA1)
    {
        return (int)(pstream - DMA1_Stream0);  // Calculate stream index for DMA1
    }
    else if (pDMAx == DMA2)
    {
        return (int)(pstream - DMA2_Stream0);  // Calculate stream index for DMA2
    }
	 return 0;
}

 DMA_Stream_TypeDef* Get_usart_DMA_Stream(USART_TypeDef *pUsart, uint8_t is_tx)
{
    uint8_t usart_num = 0;
    
    if (pUsart == USART1) usart_num = 0; 
    else if (pUsart == USART2) usart_num = 1;  
    else if (pUsart == USART3) usart_num = 2;  
    else if (pUsart == UART4) usart_num = 3;   
    else if (pUsart == UART5) usart_num = 4;  
    else if (pUsart == USART6) usart_num = 5;  

    if (usart_num != 0) 
		{
        if (is_tx) {
            return usart_dma_map[usart_num].tx_stream;
        } else {
            return usart_dma_map[usart_num].rx_stream;
        }
    }
		return 0;
}
 DMA_Stream_TypeDef* Get_adc_DMA_Stream(ADC_TypeDef *pADCx)
{
    if (pADCx == ADC1) 
		return adc1_stream;
    else 
		return adc2_stream;
}

DMA_Stream_TypeDef* Get_i2c_DMA_Stream(I2C_TypeDef *pI2c, uint8_t is_tx)
{
    uint8_t i2c_num = 0;

    if (pI2c == I2C1) i2c_num = 0;  // I2C1 -> 0
    else if (pI2c == I2C2) i2c_num = 1;  // I2C2 -> 1
    else if (pI2c == I2C3) i2c_num = 2;  // I2C3 -> 2

    // Return the appropriate DMA stream (TX or RX)
    if (i2c_num != 0) {
        if (is_tx) {
            return i2c_dma_map[i2c_num].tx_stream;
        } else {
            return i2c_dma_map[i2c_num].rx_stream;
        }
    }
    return 0;
}


/*------------------------only send arrays---------------------------*/
void Dma_Usart_Send(USART_TypeDef *pUsart,uint32_t *Source_addr)
{
		uint8_t is_tx = 1;               // 1 for TX, 0 for RX
		DMA_Stream_TypeDef *pstream;
		pstream = Get_usart_DMA_Stream(pUsart, is_tx);
		pUsart->CR3|=(1<<7);
		pstream->CR &= ~(1 << 0);
    while (pstream->CR & (1 << 0));
		//DIRECTION IS m2p
		pstream->M0AR =(uint32_t)Source_addr;
		pstream->PAR =(uint32_t)&pUsart->DR;
		//Increment mode for memory 
		pstream->CR|=(1<<10);
		//pstream->NDTR = (sizeof(Source_addr)-1);
	  //pstream->NDTR = strlen((char *)Source_addr);
		pstream->NDTR = 1;
		pstream->CR |= DMA_SxCR_CIRC;
		pstream->CR |=(1<<0);
}

void Dma_Usart_Send_array(USART_TypeDef *pUsart,uint32_t *Source_addr,uint8_t size)
{
		uint8_t is_tx = 1;               // 1 for TX, 0 for RX
		DMA_Stream_TypeDef *pstream;
		pstream = Get_usart_DMA_Stream(pUsart, is_tx);
		pUsart->CR3|=(1<<7);
		pstream->CR &= ~(1 << 0);
    while (pstream->CR & (1 << 0));
		//DIRECTION IS m2p
		pstream->M0AR =(uint32_t)Source_addr;
		pstream->PAR =(uint32_t)&pUsart->DR;
		//Increment mode for memory 
		pstream->CR|=(1<<10);
		//pstream->NDTR = (sizeof(Source_addr)-1);
	  //pstream->NDTR = strlen((char *)Source_addr);
		pstream->NDTR = size;
		pstream->CR |= DMA_SxCR_CIRC;
		pstream->CR |=(1<<0);
}


/*------------------------------------------- ADC -----------------------------------------------------*/

void dma_adc(ADC_TypeDef *pADCx, uint16_t *dataBuffer,uint8_t dataLength)
{
		DMA_Stream_TypeDef *pstream = Get_adc_DMA_Stream(pADCx);
    pstream->CR &= ~DMA_SxCR_EN;
		while (pstream->CR & DMA_SxCR_EN);
		pstream->CR &= ~(DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE);           
    pstream->CR |= DMA_SxCR_MINC;          
    pstream->CR &= ~DMA_SxCR_PINC;          
    pstream->CR |= (2 << DMA_SxCR_PL_Pos);  
		pstream->NDTR = dataLength;
    pstream->PAR = (uint32_t)&pADCx->DR;   
    pstream->M0AR =(uint32_t)dataBuffer;
		pstream->CR |= DMA_SxCR_CIRC;
		
    pstream->CR |= DMA_SxCR_EN;
    
	pADCx->CR2 |= (1 << 30); // SWSTART
}

//void Start_ADC_DMA(uint16_t *buffer, uint8_t length)
//{
//    

//    // 2. Set peripheral and memory addresses
//    adc1_stream->PAR = (uint32_t)&ADC1->DR;
//    adc1_stream->M0AR = (uint32_t)buffer;

//		// 1. Set number of data items to transfer
//    DMA2_Stream4->NDTR = 1;
//	
//    // 3. Enable DMA stream
//    adc1_stream->CR |= DMA_SxCR_EN;

//    // 4. Start ADC Conversion
//    ADC1->CR2 |= ADC_CR2_SWSTART;
//}






void Dma_I2C_Receive(I2C_TypeDef *pI2c, uint8_t *dataBuffer, uint32_t dataLength, uint8_t slaveAddress)
{
    DMA_Stream_TypeDef *pstream = Get_i2c_DMA_Stream(pI2c, 0);  // 0 for RX stream
    pI2c->CR1 |= I2C_CR1_ACK;  // Enable ACK for slave addressing

    // Disable I2C DMA for configuration
    pI2c->CR2 &= ~I2C_CR2_DMAEN;
    pstream->CR &= ~DMA_SxCR_EN;  // Disable DMA stream for configuration

    // Wait for DMA stream to be disabled
    while (pstream->CR & DMA_SxCR_EN);

    // Clear previous DMA flags
    if (pI2c == I2C1) {
        DMA1->LIFCR = DMA_LIFCR_CTCIF1 | DMA_LIFCR_CTEIF1 | DMA_LIFCR_CDMEIF1 | DMA_LIFCR_CFEIF1;
    } else if (pI2c == I2C2) {
        DMA1->LIFCR = DMA_LIFCR_CTCIF2 | DMA_LIFCR_CTEIF2 | DMA_LIFCR_CDMEIF2 | DMA_LIFCR_CFEIF2;
    } else if (pI2c == I2C3) {
        DMA2->LIFCR = DMA_LIFCR_CTCIF3 | DMA_LIFCR_CTEIF3 | DMA_LIFCR_CDMEIF3 | DMA_LIFCR_CFEIF3;
    }

    // Disable DMA interrupts (if not already disabled)
    pstream->CR &= ~(DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE);

    // Configure DMA for the I2C receive
    pstream->CR |= DMA_SxCR_MINC;  // Memory increment mode
    pstream->CR &= ~DMA_SxCR_PINC;  // Peripheral increment mode (not required for I2C)
    pstream->CR |= (2 << DMA_SxCR_PL_Pos);  // Set priority level (medium)

    // Set the number of data to be transferred
    pstream->NDTR = dataLength;

    // Set the peripheral address (I2C DR register)
    pstream->PAR = (uint32_t)&pI2c->DR;

    // Set the memory address (dataBuffer)
    pstream->M0AR = (uint32_t)dataBuffer;

    // Enable circular mode if you want continuous reception (optional)
     pstream->CR |= DMA_SxCR_CIRC;  

    // Enable DMA for I2C peripheral
    pI2c->CR2 |= I2C_CR2_DMAEN;
		NVIC_EnableIRQ(DMA1_Stream0_IRQn);  // For DMA1 Stream 1 (I2C1 RX DMA)

    // Start the I2C communication by sending the slave address
    pI2c->CR1 |= I2C_CR1_START;  // Send start condition
    pI2c->DR = slaveAddress << 1;  // Write slave address (7-bit left-shifted)
    
    // Enable DMA stream
    pstream->CR |= DMA_SxCR_EN;
}


void Dma_I2C_Send(I2C_TypeDef *pI2c, uint8_t *dataBuffer, uint32_t dataLength, uint8_t slaveAddress)
{
    DMA_Stream_TypeDef *pstream = Get_i2c_DMA_Stream(pI2c, 1);  // 1 for TX stream
    pI2c->CR1 |= I2C_CR1_ACK;  // Enable ACK for slave addressing

    // Disable I2C DMA for configuration
    pI2c->CR2 &= ~I2C_CR2_DMAEN;
    pstream->CR &= ~DMA_SxCR_EN;  // Disable DMA stream for configuration

    // Wait for DMA stream to be disabled
    while (pstream->CR & DMA_SxCR_EN);

    // Clear previous DMA flags
    if (pI2c == I2C1) {
        DMA1->LIFCR = DMA_LIFCR_CTCIF1 | DMA_LIFCR_CTEIF1 | DMA_LIFCR_CDMEIF1 | DMA_LIFCR_CFEIF1;
    } else if (pI2c == I2C2) {
        DMA1->LIFCR = DMA_LIFCR_CTCIF2 | DMA_LIFCR_CTEIF2 | DMA_LIFCR_CDMEIF2 | DMA_LIFCR_CFEIF2;
    } else if (pI2c == I2C3) {
        DMA2->LIFCR = DMA_LIFCR_CTCIF3 | DMA_LIFCR_CTEIF3 | DMA_LIFCR_CDMEIF3 | DMA_LIFCR_CFEIF3;
    }

    // Disable DMA interrupts (if not already disabled)
    pstream->CR &= ~(DMA_SxCR_TCIE | DMA_SxCR_TEIE | DMA_SxCR_DMEIE);

    // Set the number of data to be transferred
    pstream->NDTR = dataLength;

    // Set the peripheral address (I2C DR register)
    pstream->PAR = (uint32_t)&pI2c->DR;

    // Set the memory address (dataBuffer)
    pstream->M0AR = (uint32_t)dataBuffer;

    // Enable DMA for I2C peripheral
    pI2c->CR2 |= I2C_CR2_DMAEN;

    // Enable the DMA stream for I2C TX
    pstream->CR |= DMA_SxCR_EN;

    // Start the I2C communication by sending the slave address
    pI2c->CR1 |= I2C_CR1_START;  // Send start condition
		
		
    pI2c->DR = slaveAddress << 1;  // Write slave address (7-bit left-shifted)
}



void DMA1_Stream0_IRQHandler(void)
{
    if (DMA1->HISR & (1 << 5)) {  // Check if transfer complete
        DMA1->HIFCR = (1 << 5);  // Clear transfer complete flag
    }
}




