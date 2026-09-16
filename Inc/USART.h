#include <stm32f407xx.h>

#ifndef _USART_H
#define _USART_H
//Clock Enable Macros
#define USART1_PCLK_EN      RCC_APB2ENR|=(1<<4)
#define USART2_PCLK_EN      RCC_APB1ENR|=(1<<17)
#define USART3_PCLK_EN      RCC_APB1ENR|=(1<<18)
#define UART4_PCLK_EN       RCC_APB1ENR|=(1<<19)
#define UART5_PCLK_EN       RCC_APB1ENR|=(1<<20)
#define USART6_PCLK_EN      RCC_APB2ENR|=(1<<5)

//Enable Disable macros
#define USART1_EN           USART1->CR1|=(1<<13)
#define USART2_EN           USART2->CR1|=(1<<13)
#define USART3_EN           USART3->CR1|=(1<<13)
#define UART4_EN            UART4->CR1|=(1<<13)
#define UART5_EN            UART5->CR1|=(1<<13)
#define USART6_EN           USART6->CR1|=(1<<13)
/*
 *@USART_Mode
 *Possible options for USART_Mode
 */
#define _TX 0
#define _RX 1
#define _TXRX  2

/*
 *@USART_Baud
 *Possible options for USART_Baud
 */
#define BAUD_9600					96
#define BAUD_19200 				192
#define BAUD_38400 				384
#define BAUD_57600 				576
#define BAUD_115200 			1152
#define BAUD_230400 			2304
#define BAUD_460800 			4608
#define BAUD_921600 			9216
#define BAUD_1M 					10000
#define BAUD_2M 					20000
#define BAUD_3M 					30000

/*
 *@USART_ParityControl
 *Possible options for USART_ParityControl
 */
#define PARITY_ODD   2
#define PARITY_EVEN  1
#define PARITY_DISABLE   0

/*
 *@USART_WordLength
 *Possible options for USART_WordLength
 */
#define USART_8BITS  0
#define USART_9BITS  1

/*
 *@USART_NoOfStopBits
 *Possible options for USART_NoOfStopBits
 */
#define STOPBITS_1     0
#define STOPBITS_0_5   1
#define STOPBITS_2     2
#define STOPBITS_1_5   3

/*
 *@USART_HWFlowControl
 *Possible options for USART_HWFlowControl
 */
#define USART_HW_FLOW_CTRL_NONE    	0
#define USART_HW_FLOW_CTRL_CTS    	1
#define USART_HW_FLOW_CTRL_RTS    	2
#define USART_HW_FLOW_CTRL_CTS_RTS	3

typedef struct
{
	uint8_t USART_Mode;               //USART_Mode
	uint32_t USART_Baud;              //USART_Baud
	uint8_t USART_NoOfStopBits;       //USART_NoOfStopBits
	uint8_t USART_WordLength;         //USART_WordLength
	uint8_t USART_ParityControl;      //USART_Parity
	uint8_t USART_HWFlowControl;      //USART_HWFlowControl
}USART_Config_t;
typedef struct
{
	//First elemant is base address of GPIO pionter
  USART_TypeDef *pUSARTx;
	USART_Config_t USART_Config;
}USART_Handle_t;


extern double uart_timeout;
/******************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/
/*
 * Peripheral Clock setup
 */
void USART_PeriClockControl(USART_TypeDef *pUSARTx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_TypeDef *pUSARTx);


/*
 * Data Send and Receive
 */
void USART_SendData(USART_TypeDef *pUSARTx,uint8_t pTxBuffer);
//void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveData(USART_TypeDef *pUSARTx);
uint8_t USART_receive(USART_TypeDef *pUSARTx);
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);
void USART_Send_uint8_string (USART_TypeDef *pUSARTHandle,uint8_t *Uart_string,int len);
void USART_Send_string(USART_TypeDef *pUSARTHandle,char *pTxbuffer);
void USART_SendData_char(USART_TypeDef *pUSARTHandle,char pTxBuffer);
void USART_Send_array(USART_TypeDef *pUSARTHandle,char *pTxbuffer);


//// test function for odrive 
uint8_t USART_ReceiveData_From_Odrive(USART_TypeDef *pUSARTx);




/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void USART_IRQHandling(USART_Handle_t *pHandle);

/*
 * Other Peripheral Control APIs
 */
void USART_PeripheralControl(USART_TypeDef *pUSARTx, uint8_t EnOrDi);
#endif
