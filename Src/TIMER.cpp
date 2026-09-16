#include <stm32f407xx.h>
#include "stm32f407xx_1.h"
#include "TIMER.h"
//#include "common.h"

void Timer_Init(Timer_Handle_t *pTimerHandler)
{
	if(pTimerHandler->Timer_Config.Mode==0)
	{
		TimerPeriClock(pTimerHandler->pTIMx,ENABLE);
		pTimerHandler->pTIMx->PSC=(uint16_t)(pTimerHandler->Timer_Config.prescaler+1);
		pTimerHandler->pTIMx->ARR=(uint16_t)(pTimerHandler->Timer_Config.period+1);
		pTimerHandler->pTIMx->CR1|=(1<<7);
		pTimerHandler->pTIMx->DIER|=(1<<0);
	}
	else if(pTimerHandler->Timer_Config.Mode==1)
	{
		pTimerHandler->pTIMx->SMCR|=(pTimerHandler->Timer_Config.CapEdge<<0);
		pTimerHandler->pTIMx->CCER= 0;
		pTimerHandler->pTIMx->CCMR1 = 0;
		pTimerHandler->pTIMx->CCMR1 |=(0x01<<0)|(0x01<<8);
		pTimerHandler->pTIMx->ARR=(uint32_t)(pTimerHandler->Timer_Config.period+1);
		//pTimerHandler->pTIMx->DIER=0x00000009;
	}
}
void TimerPeriClock(TIM_TypeDef *pTIMx,uint8_t EnorDi)
{
	if(EnorDi==ENABLE)
	{
		switch((int)pTIMx)
		{
			case (int)TIM1: 			
		  RCC->APB2ENR|=(1<<0);
			break;
	
			case (int)TIM2: 			
		  RCC->APB1ENR|=(1<<0);
			break;
			
			case (int)TIM3:
			RCC->APB1ENR|=(1<<1);
			break;
			
			case (int)TIM4:
			RCC->APB1ENR|=(1<<2);
      break;
			
			case (int)TIM5:
			RCC->APB1ENR|=(1<<3);
			break;
			
			case (int)TIM6: 			
		  RCC->APB1ENR|=(1<<4);
			break;

			
			case (int)TIM7:
			RCC->APB1ENR|=(1<<5);
      break;
			
			case (int)TIM8:
			RCC->APB2ENR|=(1<<1);
      break;
			
			case (int)TIM9:
			RCC->APB2ENR|=(1<<16);
      break;
			
			case (int)TIM10:
			RCC->APB2ENR|=(1<<17);
      break;
			
			case (int)TIM11:
			RCC->APB2ENR|=(1<<18);
      break;
			
			case (int)TIM12:
			RCC->APB1ENR|=(1<<6);
      break;
			
			case (int)TIM13:
			RCC->APB1ENR|=(1<<7);
      break;
			
			case (int)TIM14:
			RCC->APB1ENR|=(1<<8);
      break;
			
		}
		
		
	}
	else 
	{
	}
}



void Timer_IRQconfig(uint8_t IRQNumber,uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
{
    if (IRQNumber <= 31)
        *NVIC_ISER0 = (1 << IRQNumber);
    else if (IRQNumber < 64)
        *NVIC_ISER1 = (1 << (IRQNumber - 32));
    else if (IRQNumber < 96)
        *NVIC_ISER2 = (1 << (IRQNumber - 64));
}
else
{
    if (IRQNumber <= 31)
        *NVIC_ICER0 = (1 << IRQNumber);
    else if (IRQNumber < 64)
        *NVIC_ICER1 = (1 << (IRQNumber - 32));
    else if (IRQNumber < 96)
        *NVIC_ICER2 = (1 << (IRQNumber - 64));
}

}

void TIMER_IRQPriority(uint8_t IRQNum, uint8_t IRQprio)
{
    uint8_t ipr_index = IRQNum / 4;      // which IPR register
    uint8_t section   = IRQNum % 4;      // which byte inside IPR
    uint8_t shift     = (section * 8) + 4; // STM32 uses upper 4 bits

    NVIC_IPR_BASE[ipr_index] &= ~(0xF << shift);        // clear
    NVIC_IPR_BASE[ipr_index] |=  (IRQprio << shift);   // set
}
void delay_con()
{
	TimerPeriClock(TIM14,ENABLE);
	//TIM12->CR1&=~(0xFFFF);
	TIM14->PSC=0x54;
	TIM14->CR1|=(1<<3);
	TIM14->CR1|=(1<<7);
}

void waitus(uint16_t delayus)
{
	TIM14->ARR=delayus-2;
	TIM14->CR1|=(1<<0);
  while(!(TIM14->SR&0x01));
  TIM14->SR&=~(1<<0);
}		

void waitms(uint16_t delayms)
{
	while(delayms--)
	{
		waitus(1000);
		
	}
}
