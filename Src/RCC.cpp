#include "RCC.h"
#include <stm32f407xx.h>

/*
*Choose correct values of m,n,p,q for PLL
*Flash latency is too much important do not forgot to Configure it.
*/
void RCC_Init(){
	RCC->PLLCFGR&=~(0xFFFFFFFF);
	//First Enable HSE 
  RCC->CR|=(1<<16);
	//Wait untill HSE become stable and HSE ready flag set
	while(!((RCC->CR>>17)&0x1));
	//Select HSE as Source clock of PLL(Because HSE is more accurate then HSI)
	RCC->PLLCFGR|=(1<<22);
	//Select M bit for prescaler(2MHz is recommended in datasheet)
  RCC->PLLCFGR|=(4<<0);	//2MHz
	//Select N bit for Multiplication(N=168 PLL=336)
  RCC->PLLCFGR|=(0xA8<<6);  	
	//Select P bit for prescaler(P=2)
	RCC->PLLCFGR|=(0<<16);
	//Select Q bit for prescaler(it should not exceed 48MHz)
	RCC->PLLCFGR|=(0x07<<24);
	//Assign Flash latency
	FLASH->ACR|=(5<<0);
	//Turn-On PLL
	RCC->CR|=(1<<24);
	//Wait untill PLL become stable
	while(!((RCC->CR>>25)&0x01));
	//Select VOS bit to drive at 168MHz
	RCC->APB1ENR|=(1<<28);//Enable Clock of PWR block
	PWR->CR&=~(0xFFFFFFF);//Clear VOS bit to reach up-to 168MHz
	//Do not change AHB1 prescaler it is already driven at 168MHz
	//Select APB1 prescelar 
	RCC->CFGR|=(0x05<<10);
	//Select APB2 prescelar 
	RCC->CFGR|=(0x04<<13);
	//Switch clock source
  RCC->CFGR|=(0x02<<0);	
}

void RCC_test()
{
   RCC->CFGR|=(0x04<<27);
   RCC->CFGR|=(0x00<<30);
}
