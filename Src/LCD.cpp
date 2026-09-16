#include <stm32f407xx.h>
#include "stm32f407xx_1.h"
#include "GPIO.h"
#include "LCD.h"
#include <math.h>

//////////////LCD_FOR_DISCOVERY//////////////

/*
SWCLK	PA14
SWDIO	PA13
*/

#define E  GPIO_10
#define Rs GPIO_8				// 0 -> Command Mode	// 1-> Data Mode
#define D4 GPIO_13			// PORT E
#define D5 GPIO_12			// PORT E
#define D6 GPIO_9			// PORT A
#define D7 GPIO_10			// PORT E

static void LCD_GPIO()
{
	GPIO_Handle_t LCD_GPIO;
	LCD_GPIO.pGPIOx=GPIOA;
	//Enable pin
	LCD_GPIO.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_OUT;
	LCD_GPIO.GPIO_PinConfig.GPIO_PinNumber=E;
	LCD_GPIO.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_LOW;
  GPIO_Init1(&LCD_GPIO); 
	//D6 pin
	LCD_GPIO.GPIO_PinConfig.GPIO_PinNumber=D6;
	GPIO_Init1(&LCD_GPIO);
	//Rs pin 
	LCD_GPIO.pGPIOx=GPIOE;
	LCD_GPIO.GPIO_PinConfig.GPIO_PinNumber=Rs;
	GPIO_Init1(&LCD_GPIO);
	//D4 pin
	LCD_GPIO.GPIO_PinConfig.GPIO_PinNumber=D4;
	GPIO_Init1(&LCD_GPIO);
	//D5 pin
	LCD_GPIO.GPIO_PinConfig.GPIO_PinNumber=D5;
	GPIO_Init1(&LCD_GPIO);
	//D7
	LCD_GPIO.GPIO_PinConfig.GPIO_PinNumber=D7;
	GPIO_Init1(&LCD_GPIO);
}

static void LCD_enable()
{
    GPIO_Write(GPIOA,E,SET);
	  for(int t=0;t<50000;t++);
    
	  GPIO_Write(GPIOA,E,RESET);
	  for(int t=0;t<50000;t++);
}

void ConfigLcd()
{
	  
    LCD_GPIO();
    GPIO_Write(GPIOA,E,SET);
    GPIO_Write(GPIOE,Rs,RESET);
	
    LCD_command(0x33);		// Function set to 8-bit mode
    LCD_command(0x32);		// Function set to 8-bit mode again
    LCD_command(0x28);		// 2 lines, 4-bit mode
	  LCD_command(0x0E);		// Display on, cursor blinking
	  LCD_command(0x01);		// Clear the display screen
	  LCD_command(0x06);		// Shift the cursor right (e.g. data gets written in an incrementing order, left to right)
	  LCD_command(0x80);		// Force the cursor to the beginning of the 1st line
    
    for(double t=0;t<500000;t++);
	
}

void LCD_command(unsigned char command)
{
   GPIO_Write(GPIOE,Rs,RESET);
	  unsigned short int Db = 0;
    Db = ((command>>4) & 0x000F);
	if( Db &0x0001 ) GPIO_Write(GPIOE,D4,SET); else GPIO_Write(GPIOE,D4,RESET);
	if( Db &0x0002 ) GPIO_Write(GPIOE,D5,SET); else GPIO_Write(GPIOE,D5,RESET);
	if( Db &0x0004 ) GPIO_Write(GPIOA,D6,SET); else GPIO_Write(GPIOA,D6,RESET);
	if( Db &0x0008 ) GPIO_Write(GPIOE,D7,SET); else GPIO_Write(GPIOE,D7,RESET);
	  
	LCD_enable();
	Db = 0;
    Db = (command & 0x000F);
	if( Db &0x0001 ) GPIO_Write(GPIOE,D4,SET); else GPIO_Write(GPIOE,D4,RESET);
	if( Db &0x0002 ) GPIO_Write(GPIOE,D5,SET); else GPIO_Write(GPIOE,D5,RESET);
	if( Db &0x0004 ) GPIO_Write(GPIOA,D6,SET); else GPIO_Write(GPIOA,D6,RESET);
	if( Db &0x0008 ) GPIO_Write(GPIOE,D7,SET); else GPIO_Write(GPIOE,D7,RESET);

	LCD_enable();
	//waitms(1);
}

void LCD_putc(unsigned char ascii)
{
   GPIO_Write(GPIOE,Rs,SET);
    unsigned short int Db = 0;
    Db = ((ascii>>4) & 0x000F);
	if( Db &0x0001 )GPIO_Write(GPIOE,D4,SET); else GPIO_Write(GPIOE,D4,RESET);
	if( Db &0x0002 )GPIO_Write(GPIOE,D5,SET); else GPIO_Write(GPIOE,D5,RESET);
	if( Db &0x0004 )GPIO_Write(GPIOA,D6,SET); else GPIO_Write(GPIOA,D6,RESET);
	if( Db &0x0008 )GPIO_Write(GPIOE,D7,SET); else GPIO_Write(GPIOE,D7,RESET);

    LCD_enable();
    Db = 0;
    Db = (ascii & 0x000F);
	if( Db &0x0001 )GPIO_Write(GPIOE,D4,SET); else GPIO_Write(GPIOE,D4,RESET);
	if( Db &0x0002 )GPIO_Write(GPIOE,D5,SET); else GPIO_Write(GPIOE,D5,RESET);
	if( Db &0x0004 )GPIO_Write(GPIOA,D6,SET); else GPIO_Write(GPIOA,D6,RESET);
	if( Db &0x0008 )GPIO_Write(GPIOE,D7,SET); else GPIO_Write(GPIOE,D7,RESET);

	LCD_enable();
}

void lcd(char *lcd_string)
{
	while (*lcd_string)
		LCD_putc(*lcd_string++);
}


void lcd(signed int lcd_num)
{
	int tlcd_num;
	short len,sign = 0;
	char *lcd_string,num[11];
	lcd_string = num;
	//--------------------------------
	if(lcd_num == 0)
		LCD_putc('0');		
	else if(lcd_num < 0)	{
		sign = 1;
		*lcd_string++ = '-';
		lcd_num = -lcd_num;
	}
	//--------------------------------
	tlcd_num = lcd_num;							  
	for(len=0;tlcd_num>0;len++,tlcd_num/=10);
	lcd_string += len;
	*lcd_string-- = 0;
	for(;len>0;len--)	{
		*lcd_string-- = (lcd_num%10) + 48;
		lcd_num /= 10;
	}
	if(sign == 1)
		lcd_string--;
	lcd_string++;
	//--------------------------------
	while (*lcd_string) 
		LCD_putc(*lcd_string++);
}


void lcd(unsigned int lcd_num)
{
	int tlcd_num;
	short len;
	char *lcd_string,num[11];
	lcd_string = num;
	//--------------------------------
	if(lcd_num == 0)
		LCD_putc('0');
	//--------------------------------
	tlcd_num = lcd_num;
	for(len=0;tlcd_num>0;len++,tlcd_num/=10);
	lcd_string += len;
	*lcd_string-- = 0;
	for(;len>0;len--)	{
		*lcd_string-- = lcd_num%10 + 48;
		lcd_num /= 10;	
	}
	lcd_string++;
	//--------------------------------
	while (*lcd_string)
		LCD_putc(*lcd_string++);
}


void lcd(float lcd_num,unsigned short fdigit)
{
	int floating;
	int natural = (int)lcd_num;
	float tfloating;
	if(lcd_num >= 0) {	 
		tfloating = (lcd_num-(float)natural);
	}
	else {		
		tfloating = -(lcd_num-(float)natural);
	}			

	//--------------------------------
	if(lcd_num<0 && natural==0)
	{
		LCD_putc('-');
 	}
	//--------------------------------

	lcd(natural);
	LCD_putc('.');
	
	tfloating *= pow((float)10,fdigit);
	floating = (int)tfloating;
	//--------------------------------
	while(fdigit && floating)	{
		
		--fdigit;
		floating /= 10;
	}

	while(fdigit)	{

		--fdigit;
		LCD_putc('0');
	}
	//--------------------------------
	floating = (int)tfloating;
	lcd(floating);
}



void lcd(double lcd_num,unsigned short fdigit)
{
	int floating;
	int natural = (int)lcd_num;
	double tfloating;
    if(lcd_num >= 0) {	 
		tfloating = (lcd_num-(double)natural);
	}
	else {		
		tfloating = -(lcd_num-(double)natural);
	}			

	//--------------------------------
	if(lcd_num<0 && natural==0)
	{
		LCD_putc('-');
 	}
	//--------------------------------

	lcd(natural);
	LCD_putc('.');

	tfloating *= pow((double)10,fdigit);
	floating = (int)tfloating;
	//--------------------------------
	while(fdigit && floating)	{
		
		--fdigit;
		floating /= 10;
	}

	while(fdigit)	{

		--fdigit;
		LCD_putc('0');
	}
	//--------------------------------
	floating = (int)tfloating;
	lcd(floating);
}