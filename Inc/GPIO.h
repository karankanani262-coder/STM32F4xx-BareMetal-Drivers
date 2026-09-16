#include <stm32f407xx.h>
#ifndef _GPIO_H
#define _GPIO_H
/******************************************************************************************************
*
*                  GPIO Driver API For More Information Refer Dafination Of API                                 
*
*******************************************************************************************************/
/*
*NOTE:PA15,PA14,PA13,PB4,PB3 are in by default in JTAG so it will not work in GPIO we have to chane function of GPIO pin
*/
//GPIO Periphreal Macros
/*
*     GPIO Possible Modes(@Modes)
*/
#define GPIO_MODE_IN 			0
#define GPIO_MODE_OUT 		1
#define GPIO_MODE_ALE 		2
#define GPIO_MODE_ANA 		3
#define GPIO_MODE_IT_FT  	4
#define GPIO_MODE_IT_RT  	5
#define GPIO_MODE_IT_RFT  6

/*
*     GPIO Output Types(@Types)
*     Push-pull & Open-Drain
*/
#define GPIO_OUT_PP 0
#define GPIO_OUT_OD 1
/*
*     GPIO Output Speed(@Speed)
*/
#define GPIO_SPEED_LOW 0
#define GPIO_SPEED_MED 1
#define GPIO_SPEED_HIG 2
#define GPIO_SPEED_UHS 3
/*
*   GPIO Pull-up and Pull-down(@Pull_Up_Down)   
*/
#define GPIO_NPULL     0
#define GPIO_PUUP      1
#define GPIO_PUDW      2
/*
*   GPIO Pin Number Defination(@PINS)
*/
#define GPIO_0 	0
#define GPIO_1 	1
#define GPIO_2 	2
#define GPIO_3 	3
#define GPIO_4 	4
#define GPIO_5 	5
#define GPIO_6 	6
#define GPIO_7 	7
#define GPIO_8 	8
#define GPIO_9 	9
#define GPIO_10 10
#define GPIO_11 11
#define GPIO_12 12
#define GPIO_13 13
#define GPIO_14 14
#define GPIO_15 15 

//GPIO Enable Clock Macros
#define GPIOA_PCLK_EN      RCC_AHB1ENR|=(1<<0)
#define GPIOB_PCLK_EN      RCC_AHB1ENR|=(1<<1)
#define GPIOC_PCLK_EN      RCC_AHB1ENR|=(1<<2)
#define GPIOD_PCLK_EN      RCC_AHB1ENR|=(1<<3)
#define GPIOE_PCLK_EN      RCC_AHB1ENR|=(1<<4)
#define GPIOF_PCLK_EN      RCC_AHB1ENR|=(1<<5)
#define GPIOG_PCLK_EN      RCC_AHB1ENR|=(1<<6)
#define GPIOH_PCLK_EN      RCC_AHB1ENR|=(1<<7)
#define GPIOI_PCLK_EN      RCC_AHB1ENR|=(1<<8)

//GPIO Disable Clock Macros
#define GPIOA_PCLK_DI      RCC_AHB1ENR&=~(1<<0)
#define GPIOB_PCLK_DI      RCC_AHB1ENR&=~(1<<1)
#define GPIOC_PCLK_DI      RCC_AHB1ENR&=~(1<<2)
#define GPIOD_PCLK_DI      RCC_AHB1ENR&=~(1<<3)
#define GPIOE_PCLK_DI      RCC_AHB1ENR&=~(1<<4)
#define GPIOF_PCLK_DI      RCC_AHB1ENR&=~(1<<5)
#define GPIOG_PCLK_DI      RCC_AHB1ENR&=~(1<<6)
#define GPIOH_PCLK_DI      RCC_AHB1ENR&=~(1<<7)
#define GPIOI_PCLK_DI      RCC_AHB1ENR&=~(1<<8)

// GPIO Reset Macros
#define GPIOA_RESET       do{ RCC_AHB1RSTR|=(1<<0); RCC_AHB1RSTR&=~(1<<0);}while(0)
#define GPIOB_RESET       do{ RCC_AHB1RSTR|=(1<<1); RCC_AHB1RSTR&=~(1<<1);}while(0)
#define GPIOC_RESET       do{ RCC_AHB1RSTR|=(1<<2); RCC_AHB1RSTR&=~(1<<2);}while(0)
#define GPIOD_RESET       do{ RCC_AHB1RSTR|=(1<<3); RCC_AHB1RSTR&=~(1<<3);}while(0)
#define GPIOE_RESET       do{ RCC_AHB1RSTR|=(1<<4); RCC_AHB1RSTR&=~(1<<4);}while(0)
#define GPIOF_RESET       do{ RCC_AHB1RSTR|=(1<<5); RCC_AHB1RSTR&=~(1<<5);}while(0)
#define GPIOG_RESET       do{ RCC_AHB1RSTR|=(1<<6); RCC_AHB1RSTR&=~(1<<6);}while(0)
#define GPIOH_RESET       do{ RCC_AHB1RSTR|=(1<<7); RCC_AHB1RSTR&=~(1<<7);}while(0)
#define GPIOI_RESET       do{ RCC_AHB1RSTR|=(1<<8); RCC_AHB1RSTR&=~(1<<8);}while(0)

// GPIO Interrupt Macros(@INTPORT)
#define PA 0
#define PB 1
#define PC 2
#define PD 3
#define PE 4
#define PF 5
#define PG 6
#define PH 7


typedef struct
{
	uint8_t GPIO_PinNumber;                                   //@PINS
	uint8_t GPIO_PinPuPdControl;                              //@Pull_Up_Down
	uint8_t GPIO_PinMode;                                     //@Modes
	uint8_t GPIO_PinSpeed;                                    //@Speed
	uint8_t GPIO_PinOPtype;                                   //@Types
	uint8_t GPIO_PinAltFunMode;
	uint8_t GPIO_IntPort;                                     //@INTPORT
}GPIO_PinConfig_t;
typedef struct
{
	//First elemant is base address of GPIO pionter
  GPIO_TypeDef *pGPIOx;
	GPIO_PinConfig_t GPIO_PinConfig;
}GPIO_Handle_t;
/*
*   GPIO Peripheral Clock Functions
*/
void GPIO_Periclock_control(GPIO_TypeDef *pGPIOx,uint8_t ENorDI);
/*
*   GPIO Init and Deinit Functions
*/
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_Init1(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_TypeDef *pGPIOx);
/*
*   GPIO Read and Write Functions
*/
uint8_t GPIO_Read(GPIO_TypeDef *pGPIOx,uint8_t PinNumber);
void GPIO_Write(GPIO_TypeDef *pGPIOx,uint8_t Pinnumber,uint8_t Value);
void GPIO_Toggle(GPIO_TypeDef *pGPIOx,uint8_t PinNumber);
/*
*   GPIO IRQ Configuration and Handler Functions
*/
void GPIO_IRQconfig(uint8_t IRQNumber,uint8_t EnorDi,uint8_t PinNumber);
void GPIO_IRQHandling(uint8_t PinNumber);
void GPIO_IRQPriority(uint8_t IRQprio,uint8_t IRQNum);


#endif
