/**@By Earnest Kihara 
 *interfacing LCD 16x2 in 4BITS mode 
 */


#include "TM4C123.h"    // Device header

#define LCD GPIOB    		//LCD port with Tiva C 
#define RS 0x10  /*PA4*/
#define RW 0x20  /*PA5*/
#define EN 0x40  /*PA6*/

/*lcd pins -> PB0-PB4 */


//Functions Declaration
void delayUs(int);   														   //Delay in Micro Seconds
void delayMs(int);   														   //Delay in Milli Seconds
void LCD4bits_Init(void);													 //Initialization of LCD Dispaly
void LCD_Write4bits(unsigned char, unsigned char); //Write data as (4 bits) on LCD
void LCD_WriteString(char*);											 //Write a string on LCD 
void LCD4bits_Cmd(unsigned char);									 //Write command 
void LCD4bits_Data(unsigned char);								 //Write a character

int main(void)
{
	char* str = "Hellow World";    //Write any string you want to display on LCD
	LCD4bits_Init();									//Initialization of LCD
	LCD4bits_Cmd(0x01);								//Clear the display
	LCD4bits_Cmd(0x80);               //Force the cursor to beginning of 1st line
	delayMs(500);											//delay 500 ms for LCD (MCU is faster than LCD)
	LCD_WriteString(str);							//Write the string on LCD
	delayMs(5000);											//Delay 500 ms to let the LCD diplays the data
	
	while(1)
	{
		
		LCD4bits_Cmd(0x01);
	LCD4bits_Cmd(0x80);
	delayMs(500);
	 LCD_WriteString("mzee wa huku");
	delayMs(5000);
	LCD4bits_Cmd(0xC0);
	LCD_WriteString("upo nyumbani ");
	delayMs(5000);
		
		
	}
}

void LCD4bits_Init(void)
{
	SYSCTL->RCGCGPIO |= 0x02;    //enable clock for PORTB
	SYSCTL->RCGCGPIO |= 0x01;
	delayMs(10);                 //delay 10 ms for enable the clock of PORTB
  LCD->DIR = 0x0F;             //let PORTB as output pins
	LCD->DEN = 0x0F;             //enable PORTB digital IO pins
	GPIOA->DIR |=0xF0;
	GPIOA->DEN |=0xF0;
	LCD4bits_Cmd(0x28);          //2 lines and 5x7 character (4-bit data, D4 to D7)
	LCD4bits_Cmd(0x06);          //Automatic Increment cursor (shift cursor to right)
	LCD4bits_Cmd(0x01);					 //Clear display screen
	LCD4bits_Cmd(0x0F);          //Display on, cursor blinking
}


void LCD_Write4bits(unsigned char data, unsigned char control)
{

	GPIOA->DATA |= control;         
	LCD->DATA = data ;    
	GPIOA->DATA |= EN;
	delayUs(0);													//delay for pulsing EN
	GPIOA->DATA = 0;                      //Clear the Data 
}

void LCD_WriteString(char * str)
{  
	volatile int i = 0;          //volatile is important 
	
	while(*(str+i) != '\0')       //until the end of the string
	{
		LCD4bits_Data(*(str+i));    //Write each character of string
		i++;                        //increment for next character
	}
}

void LCD4bits_Cmd(unsigned char command)
{
	LCD_Write4bits(command >> 4 , 0);			 //then upper nibble
	LCD_Write4bits(command & 0x0F , 0);    //lower nible 
	
	
	if(command < 4)
		delayMs(2);       //commands 1 and 2 need up to 1.64ms
	else
		delayUs(40);      //all others 40 us
}

void LCD4bits_Data(unsigned char data)
{
	LCD_Write4bits(data >> 4 , RS);     //then upper nibble
	LCD_Write4bits(data & 0x0F , RS);   //lower nibble first
	
	delayUs(40);	
											//delay for LCD (MCU is faster than LCD)
}

void delayMs(int n)
{  
	volatile int i,j;             //volatile is important for variables incremented in code
	for(i=0;i<n;i++)
		for(j=0;j<3180;j++)         //delay for 1 msec
		{}
}

void delayUs(int time)             
{
	
  int i; 
	SYSCTL->RCGCTIMER = 0x01 ;    //enable clock for timer 0
	TIMER0->CTL &=(~(0x01));
	TIMER0->CFG = 0x04;
	TIMER0->TAMR = 0x02 ;
	TIMER0->TAILR = 16000 - 1; /*set load . delay = load *(1/16M) = 16000* (1/16M) = 0.001 sec*/
	TIMER0->ICR = 0X01;
	TIMER0->CTL |= 0x01;
	for( i = 0 ; i < time ; i++)
	{
		while((TIMER0->RIS & 0x01) == 0 );
		TIMER0->ICR = 0x01; /*clear interrupt */
		
	}

}

