#ifndef _STM32F407XX_H
#define _STM32F407XX_H
#include <stdint.h>
/*

Designed By Parag

*/
/**************************************************************************************************
*Basic Rules For Library
*ALL MACROS SHOULD BE IN CAPITAL LATTER
*ORDER OF STRUCTURE DECLARATION MATTERS
*GIVE APPROPIATE DEFINATION WITH @ SPECIAL CHARCTER 
*ALWAYS KEPT EXAMPLE CODE IN CPP FILE FOR REFRENCE
*ALWAYS WRITE INTERRUPT HANDLER EXTERN "C"{} NEITHER YOUR INTERRUP WILL NOT WORK("THINK REASON")
**************************************************************************************************/
//General Macros
#define ENABLE                                              1
#define DISABLE                                             0
#define SET                                                 1
#define RESET                                               0
//Base Adresses for Flash and SRAM1
#define FLASH_BASE_ADDR                                     0x08000000
#define SRAM1_BASE_ADDR                                     0x20000000
#define SRAM2_BASE_ADDR                                     0x2001C000
#define ROM_BASE_ADDR                                       0x1FFF0000
#define OTP_BASE_ADDR                                       0x1FFF7800
#define SRAM                                                0x20000000

//Peripheral bus addresses
#define APB1PERIPH_BASE_ADD                                 0x40000000
#define APB2PERIPH_BASE_ADD                                 0x40010000
#define AHB1PERIPH_BASE_ADD                                 0x40020000
#define AHB2PERIPH_BASE_ADD                                 0x50000000

//AHB1 Hanging Periperal Base Addresses
#define GPIOA_BASE_ADD                                          0x40020000
#define GPIOB_BASE_ADD                                          0x40020400
#define GPIOC_BASE_ADD                                          0x40020800
#define GPIOD_BASE_ADD                                          0x40020C00
#define GPIOE_BASE_ADD                                          0x40021000
#define GPIOF_BASE_ADD                                          0x40021400 
#define GPIOG_BASE_ADD                                          0x40021800 
#define GPIOH_BASE_ADD                                          0x40021C00
#define GPIOI_BASE_ADD                                          0x40022000
#define CRC_BASE_ADD                                            0x40023000 
#define RCC_BASE_ADD                                            0x40023800
#define FIR_BASE_ADD                                            0x40023C00
#define BKPSRAM_BASE_ADD                                        0x40024000
#define DMA1_BASE_ADD                                           0x40026000
#define DMA2_BASE_ADD                                           0x40026400
#define ETHERNET_MAC_BASE_ADD                                   0x40028000
#define USB_OTG_HS_BASE_ADD                                     0x40040000

//AHB2 Hanging Periperal Base Addresses
#define USB_OTG_FS_BASE_ADD                                     0x50000000  
#define DCMI_BASE_ADD                                           0x50050000
#define RNG_BASE_ADD                                            0x50060800

//AHB3 Hanging Periperal Base Addresses
#define FSMC_bank_1                                         0x60000000
#define FSMC_bank_2                                         0x70000000 
#define FSMC_bank_3                                         0x80000000  
#define FSMC_bank_4                                         0x90000000      
#define FSMC_CLR_REG                                        0xA0000000       

//APB1 Hanging Periperal Base Addresses
#define TIM2_BASE_ADD                                            0x40000000
#define TIM3_BASE_ADD                                            0x40000400
#define TIM4_BASE_ADD                                            0x40000800
#define TIM5_BASE_ADD                                            0x40000C00
#define TIM6_BASE_ADD                                            0x40001000
#define TIM7_BASE_ADD                                            0x40001400
#define TIM12_BASE_ADD                                           0x40001800
#define TIM13_BASE_ADD                                           0x40001C00
#define TIM14_BASE_ADD                                           0x40002000
#define RTC_BKP_BASE_ADD                                         0x40002800
#define WWDG_BASE_ADD                                            0x40002C00
#define IWDG_BASE_ADD                                            0x40003000
#define I2S2EXT_BASE_ADD                                         0x40003400
#define SPI2_BASE_ADD                                            0x40003800
#define SPI3_BASE_ADD                                            0x40003C00
#define I2S3EXT_BASE_ADD                                         0x40004000
#define USART2_BASE_ADD                                          0x40004400
#define USART3_BASE_ADD                                          0x40004800
#define UART4_BASE_ADD                                           0x40004C00 
#define UART5_BASE_ADD                                           0x40005000
#define I2C1_BASE_ADD                                            0x40005400 
#define I2C2_BASE_ADD                                            0x40005800 
#define I2C3_BASE_ADD                                            0x40005C00
#define CAN1_BASE_ADD                                            0x40006400
#define CAN2_BASE_ADD                                            0x40006800
#define PWR_BASE_ADD                                             0x40007000
#define DAC_BASE_ADD                                             0x40007400 

//APB2 Hanging Periperal Base Addresses
#define TIM1_BASE_ADD                                                 0x40010000
#define TIM8_BASE_ADD                                                 0x40010400
#define USART1_BASE_ADD                                               0x40011000
#define USART6_BASE_ADD                                               0x40011400                                        
#define ADC_1_2_3_BASE_ADD                                            0x40012000
#define SDIO_BASE_ADD                                                 0x40012C00
#define SPI1_BASE_ADD                                                 0x40013000
#define SYSCFG_BASE_ADD                                               0x40013800
#define EXTI_BASE_ADD                                                 0x40013C00 
#define TIM9_BASE_ADD                                                 0x40014000
#define TIM10_BASE_ADD                                                0x40014400
#define TIM11_BASE_ADD                                                0x40014800

/*All Base Addresses are defined above,Peripheral Control Registors Addresses are Defined below*/

//GPIOA Control Registor
#define GPIOA_MODER                               (*(volatile unsigned long *)(GPIOA_BASE_ADD + 0x00))             
#define GPIOA_OTYPER                              (*(volatile unsigned long *)(GPIOA_BASE_ADD + 0x04))
#define GPIOA_OSPEEDR                             (*(volatile unsigned long *)(GPIOA_BASE_ADD + 0x08))
#define GPIOA_PUPDR                               (*(volatile unsigned long *)(GPIOA_BASE_ADD + 0x0C))
#define GPIOA_IDR                                 (*(volatile unsigned long *)(GPIOA_BASE_ADD + 0x10))
#define GPIOA_ODR                                 (*(volatile unsigned long *)(GPIOA_BASE_ADD + 0x14))
#define GPIOA_BSRR                                (*(volatile unsigned long *)(GPIOA_BASE_ADD + 0x18))
#define GPIOA_LCKR                                (*(volatile unsigned long *)(GPIOA_BASE_ADD + 0x1C))
#define GPIOA_AFRL                                (*(volatile unsigned long *)(GPIOA_BASE_ADD + 0x20))
#define GPIOA_AFRH                                (*(volatile unsigned long *)(GPIOA_BASE_ADD + 0x24))

//GPIOB Control Registor
#define GPIOB_MODER                               (*(volatile unsigned long *)(GPIOB_BASE_ADD + 0x00))             
#define GPIOB_OTYPER                              (*(volatile unsigned long *)(GPIOB_BASE_ADD + 0x04))
#define GPIOB_OSPEEDR                             (*(volatile unsigned long *)(GPIOB_BASE_ADD + 0x08))
#define GPIOB_PUPDR                               (*(volatile unsigned long *)(GPIOB_BASE_ADD + 0x0C))
#define GPIOB_IDR                                 (*(volatile unsigned long *)(GPIOB_BASE_ADD + 0x10))
#define GPIOB_ODR                                 (*(volatile unsigned long *)(GPIOB_BASE_ADD + 0x14))
#define GPIOB_BSRR                                (*(volatile unsigned long *)(GPIOB_BASE_ADD + 0x18))
#define GPIOB_LCKR                                (*(volatile unsigned long *)(GPIOB_BASE_ADD + 0x1C))
#define GPIOB_AFRL                                (*(volatile unsigned long *)(GPIOB_BASE_ADD + 0x20))
#define GPIOB_AFRH                                (*(volatile unsigned long *)(GPIOB_BASE_ADD + 0x24))

//GPIOC Control Registor
#define GPIOC_MODER                               (*(volatile unsigned long *)(GPIOC_BASE_ADD + 0x00))             
#define GPIOC_OTYPER                              (*(volatile unsigned long *)(GPIOC_BASE_ADD + 0x04))
#define GPIOC_OSPEEDR                             (*(volatile unsigned long *)(GPIOC_BASE_ADD + 0x08))
#define GPIOC_PUPDR                               (*(volatile unsigned long *)(GPIOC_BASE_ADD + 0x0C))
#define GPIOC_IDR                                 (*(volatile unsigned long *)(GPIOC_BASE_ADD + 0x10))
#define GPIOC_ODR                                 (*(volatile unsigned long *)(GPIOC_BASE_ADD + 0x14))
#define GPIOC_BSRR                                (*(volatile unsigned long *)(GPIOC_BASE_ADD + 0x18))
#define GPIOC_LCKR                                (*(volatile unsigned long *)(GPIOC_BASE_ADD + 0x1C))
#define GPIOC_AFRL                                (*(volatile unsigned long *)(GPIOC_BASE_ADD + 0x20))
#define GPIOC_AFRH                                (*(volatile unsigned long *)(GPIOC_BASE_ADD + 0x24))

//GPIOD Control Registor
#define GPIOD_MODER                               (*(volatile unsigned long *)(GPIOD_BASE_ADD + 0x00))             
#define GPIOD_OTYPER                              (*(volatile unsigned long *)(GPIOD_BASE_ADD + 0x04))
#define GPIOD_OSPEEDR                             (*(volatile unsigned long *)(GPIOD_BASE_ADD + 0x08))
#define GPIOD_PUPDR                               (*(volatile unsigned long *)(GPIOD_BASE_ADD + 0x0C))
#define GPIOD_IDR                                 (*(volatile unsigned long *)(GPIOD_BASE_ADD + 0x10))
#define GPIOD_ODR                                 (*(volatile unsigned long *)(GPIOD_BASE_ADD + 0x14))
#define GPIOD_BSRR                                (*(volatile unsigned long *)(GPIOD_BASE_ADD + 0x18))
#define GPIOD_LCKR                                (*(volatile unsigned long *)(GPIOD_BASE_ADD + 0x1C))
#define GPIOD_AFRL                                (*(volatile unsigned long *)(GPIOD_BASE_ADD + 0x20))
#define GPIOD_AFRH                                (*(volatile unsigned long *)(GPIOD_BASE_ADD + 0x24))

//GPIOE Control Registor
#define GPIOE_MODER                               (*(volatile unsigned long *)(GPIOE_BASE_ADD + 0x00))             
#define GPIOE_OTYPER                              (*(volatile unsigned long *)(GPIOE_BASE_ADD + 0x04))
#define GPIOE_OSPEEDR                             (*(volatile unsigned long *)(GPIOE_BASE_ADD + 0x08))
#define GPIOE_PUPDR                               (*(volatile unsigned long *)(GPIOE_BASE_ADD + 0x0C))
#define GPIOE_IDR                                 (*(volatile unsigned long *)(GPIOE_BASE_ADD + 0x10))
#define GPIOE_ODR                                 (*(volatile unsigned long *)(GPIOE_BASE_ADD + 0x14))
#define GPIOE_BSRR                                (*(volatile unsigned long *)(GPIOE_BASE_ADD + 0x18))
#define GPIOE_LCKR                                (*(volatile unsigned long *)(GPIOE_BASE_ADD + 0x1C))
#define GPIOE_AFRL                                (*(volatile unsigned long *)(GPIOE_BASE_ADD + 0x20))
#define GPIOE_AFRH                                (*(volatile unsigned long *)(GPIOE_BASE_ADD + 0x24))

//GPIOF Control Registor
#define GPIOF_MODER                               (*(volatile unsigned long *)(GPIOF_BASE_ADD + 0x00))             
#define GPIOF_OTYPER                              (*(volatile unsigned long *)(GPIOF_BASE_ADD + 0x04))
#define GPIOF_OSPEEDR                             (*(volatile unsigned long *)(GPIOF_BASE_ADD + 0x08))
#define GPIOF_PUPDR                               (*(volatile unsigned long *)(GPIOF_BASE_ADD + 0x0C))
#define GPIOF_IDR                                 (*(volatile unsigned long *)(GPIOF_BASE_ADD + 0x10))
#define GPIOF_ODR                                 (*(volatile unsigned long *)(GPIOF_BASE_ADD + 0x14))
#define GPIOF_BSRR                                (*(volatile unsigned long *)(GPIOF_BASE_ADD + 0x18))
#define GPIOF_LCKR                                (*(volatile unsigned long *)(GPIOF_BASE_ADD + 0x1C))
#define GPIOF_AFRL                                (*(volatile unsigned long *)(GPIOF_BASE_ADD + 0x20))
#define GPIOF_AFRH                                (*(volatile unsigned long *)(GPIOF_BASE_ADD + 0x24))

//GPIOG Control Registor
#define GPIOG_MODER                               (*(volatile unsigned long *)(GPIOG_BASE_ADD + 0x00))             
#define GPIOG_OTYPER                              (*(volatile unsigned long *)(GPIOG_BASE_ADD + 0x04))
#define GPIOG_OSPEEDR                             (*(volatile unsigned long *)(GPIOG_BASE_ADD + 0x08))
#define GPIOG_PUPDR                               (*(volatile unsigned long *)(GPIOG_BASE_ADD + 0x0C))
#define GPIOG_IDR                                 (*(volatile unsigned long *)(GPIOG_BASE_ADD + 0x10))
#define GPIOG_ODR                                 (*(volatile unsigned long *)(GPIOG_BASE_ADD + 0x14))
#define GPIOG_BSRR                                (*(volatile unsigned long *)(GPIOG_BASE_ADD + 0x18))
#define GPIOG_LCKR                                (*(volatile unsigned long *)(GPIOG_BASE_ADD + 0x1C))
#define GPIOG_AFRL                                (*(volatile unsigned long *)(GPIOG_BASE_ADD + 0x20))
#define GPIOG_AFRH                                (*(volatile unsigned long *)(GPIOG_BASE_ADD + 0x24))

//GPIOH Control Registor
#define GPIOH_MODER                               (*(volatile unsigned long *)(GPIOH_BASE_ADD + 0x00))             
#define GPIOH_OTYPER                              (*(volatile unsigned long *)(GPIOH_BASE_ADD + 0x04))
#define GPIOH_OSPEEDR                             (*(volatile unsigned long *)(GPIOH_BASE_ADD + 0x08))
#define GPIOH_PUPDR                               (*(volatile unsigned long *)(GPIOH_BASE_ADD + 0x0C))
#define GPIOH_IDR                                 (*(volatile unsigned long *)(GPIOH_BASE_ADD + 0x10))
#define GPIOH_ODR                                 (*(volatile unsigned long *)(GPIOH_BASE_ADD + 0x14))
#define GPIOH_BSRR                                (*(volatile unsigned long *)(GPIOH_BASE_ADD + 0x18))
#define GPIOH_LCKR                                (*(volatile unsigned long *)(GPIOH_BASE_ADD + 0x1C))
#define GPIOH_AFRL                                (*(volatile unsigned long *)(GPIOH_BASE_ADD + 0x20))
#define GPIOH_AFRH                                (*(volatile unsigned long *)(GPIOH_BASE_ADD + 0x24))

//GPIOI Control Registor
#define GPIOI_MODER                               (*(volatile unsigned long *)(GPIOI_BASE_ADD + 0x00))             
#define GPIOI_OTYPER                              (*(volatile unsigned long *)(GPIOI_BASE_ADD + 0x04))
#define GPIOI_OSPEEDR                             (*(volatile unsigned long *)(GPIOI_BASE_ADD + 0x08))
#define GPIOI_PUPDR                               (*(volatile unsigned long *)(GPIOI_BASE_ADD + 0x0C))
#define GPIOI_IDR                                 (*(volatile unsigned long *)(GPIOI_BASE_ADD + 0x10))
#define GPIOI_ODR                                 (*(volatile unsigned long *)(GPIOI_BASE_ADD + 0x14))
#define GPIOI_BSRR                                (*(volatile unsigned long *)(GPIOI_BASE_ADD + 0x18))
#define GPIOI_LCKR                                (*(volatile unsigned long *)(GPIOI_BASE_ADD + 0x1C))
#define GPIOI_AFRL                                (*(volatile unsigned long *)(GPIOI_BASE_ADD + 0x20))
#define GPIOI_AFRH                                (*(volatile unsigned long *)(GPIOI_BASE_ADD + 0x24))

//RCC Engine Control Registor
#define RCC_CR                                    (*(volatile unsigned long *)(RCC_BASE_ADD + 0x00))   
#define RCC_PLLCFGR                               (*(volatile unsigned long *)(RCC_BASE_ADD + 0x04)) 
#define RCC_CFGR                                  (*(volatile unsigned long *)(RCC_BASE_ADD + 0x08)) 
#define RCC_CIR                                   (*(volatile unsigned long *)(RCC_BASE_ADD + 0x0C)) 
#define RCC_AHB1RSTR                              (*(volatile unsigned long *)(RCC_BASE_ADD + 0x10)) 
#define RCC_AHB2RSTR                              (*(volatile unsigned long *)(RCC_BASE_ADD + 0x14))   
#define RCC_AHB3RSTR                              (*(volatile unsigned long *)(RCC_BASE_ADD + 0x18))    
#define RCC_APB1RSTR                              (*(volatile unsigned long *)(RCC_BASE_ADD + 0x20))  
#define RCC_APB2RSTR                              (*(volatile unsigned long *)(RCC_BASE_ADD + 0x24))   
#define RCC_AHB1ENR                               (*(volatile unsigned long *)(RCC_BASE_ADD + 0x30))               
#define RCC_AHB2ENR                               (*(volatile unsigned long *)(RCC_BASE_ADD + 0x34))  
#define RCC_AHB3ENR                               (*(volatile unsigned long *)(RCC_BASE_ADD + 0x38))  
#define RCC_APB1ENR                               (*(volatile unsigned long *)(RCC_BASE_ADD + 0x40))  
#define RCC_APB2ENR                               (*(volatile unsigned long *)(RCC_BASE_ADD + 0x44)) 
#define RCC_AHB1LPENR                             (*(volatile unsigned long *)(RCC_BASE_ADD + 0x50)) 
#define RCC_AHB2LPENR                             (*(volatile unsigned long *)(RCC_BASE_ADD + 0x54)) 
#define RCC_AHB3LPENR                             (*(volatile unsigned long *)(RCC_BASE_ADD + 0x58)) 
#define RCC_APB1LPENR                             (*(volatile unsigned long *)(RCC_BASE_ADD + 0x60)) 
#define RCC_APB2LPENR                             (*(volatile unsigned long *)(RCC_BASE_ADD + 0x64)) 
#define RCC_BDCR                                  (*(volatile unsigned long *)(RCC_BASE_ADD + 0x70)) 
#define RCC_CSR                                   (*(volatile unsigned long *)(RCC_BASE_ADD + 0x74)) 
#define RCC_SSCGR                                 (*(volatile unsigned long *)(RCC_BASE_ADD + 0x80)) 
#define RCC_PLLI2SCFGR                            (*(volatile unsigned long *)(RCC_BASE_ADD + 0x84)) 

#define NVIC_BASE_ADDR 0xE000E004

#define NVIC_ISER0  ((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1  ((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2  ((volatile uint32_t*)0xE000E108)
#define NVIC_ISER3  ((volatile uint32_t*)0xE000E10C)
	
#define NVIC_ICER0  ((volatile uint32_t*)0xE000E180)
#define NVIC_ICER1  ((volatile uint32_t*)0xE000E184)
#define NVIC_ICER2  ((volatile uint32_t*)0xE000E188)
#define NVIC_ICER3  ((volatile uint32_t*)0xE000E18C)
	
#define NVIC_IPR_BASE  ((volatile uint32_t*)0xE000E400)

#define SYS_EXTICR1 (*(volatile unsigned long *)(SYSCFG_BASE_ADD + 0x08)) 
#define SYS_EXTICR2 (*(volatile unsigned long *)(SYSCFG_BASE_ADD + 0x0C)) 
#define SYS_EXTICR3 (*(volatile unsigned long *)(SYSCFG_BASE_ADD + 0x10)) 
#define SYS_EXTICR4 (*(volatile unsigned long *)(SYSCFG_BASE_ADD + 0x14)) 




#endif


