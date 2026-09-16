#include <stm32f407xx.h>
#ifndef _TIMER_H
#define _TIMER_H
//Timer Enable Macros
#define TIMER1_ENABLE  TIM1->CR1|=(1<<0)
#define TIMER6_ENABLE  TIM6->CR1|=(1<<0)
#define TIMER2_ENABLE  TIM2->CR1|=(1<<0)
#define TIMER3_ENABLE  TIM3->CR1|=(1<<0)
#define TIMER4_ENABLE  TIM4->CR1|=(1<<0)
#define TIMER5_ENABLE  TIM5->CR1|=(1<<0)
#define TIMER7_ENABLE  TIM7->CR1|=(1<<0)
#define TIMER8_ENABLE  TIM8->CR1|=(1<<0)
#define TIMER9_ENABLE  TIM9->CR1|=(1<<0)
#define TIMER10_ENABLE  TIM10->CR1|=(1<<0)
#define TIMER12_ENABLE  TIM12->CR1|=(1<<0)
#define TIMER13_ENABLE  TIM13->CR1|=(1<<0)

#define TIMER12_DISABLE  TIM12->CR1&=~(1<<0)
#define TIMER13_DISABLE  TIM13->CR1&=~(1<<0)

//Capture Edge Macros(@MODE)
#define RISING     1
#define FALLING    2
#define BOTH       3



typedef struct
{
	uint8_t Mode;      
	uint8_t CapEdge;    //@MODE
	uint32_t prescaler;
	uint32_t period;
  uint8_t  duty;
  uint8_t pwm_c;	
}Timer_Config_t;


typedef struct
{
	Timer_Config_t Timer_Config;
	TIM_TypeDef *pTIMx;
}Timer_Handle_t;

void Timer_Init(Timer_Handle_t *pTimerHandler);
void TimerPeriClock(TIM_TypeDef *pTIMx,uint8_t EnorDi);
void Timer_IRQconfig(uint8_t IRQNumber,uint8_t EnorDi);
void waitus(uint16_t delayus);
void waitms(uint16_t delayms);
void delay_con();
void TIMER_IRQPriority(uint8_t IRQNum, uint8_t IRQprio);

#endif
