/*
 * toll_project_final0.cpp
 *
 * Created: 18-08-2019 13:30:09
 * Author : aastha
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "lcd16x2.h"
#include "usart.h"
#include <avr/interrupt.h>

serialcom uart;
LiquidCrystal lcd;

int match(char str1[],char str2[])
{
	int m=1;
	for (int i=0;i<12;i++){
		if (str1[i]!=str2[i]){
			m=0;
		}
	}
	return m;
}
/*int match_pin(int str1[],int str2[])
{
	int m=2;
	for (int i=0;i<4;i++){
		if (str1[i]!=str2[i]){
			m=4;
		}
	}
	return m;
}*/

int match_pin(int str1,int str2)
{
	if (str1==str2){
		return 1;
	}
	else return 0;
}

volatile int i=0,r=0; char tag[12];char pin[4];
ISR(USART_RXC_vect){
	tag[i]=UDR;
	i++;
	if (i==12){
		uart.print(  tag);
		i=0;r=1;
	}
}

void nullTag(char tag[])
{
	for (i=0;i<12;i++)
	{
		tag[i]='\0';
	}
}

int keys[4][4] = {{7,4,1,12},{8,5,2,0},{9,6,3,11},{12,13,14,15}};
int getKey()
{
	DDRB = 0x0F;
	for (int i  = 1,c=0;i<=8;i*=2,c++)
	{
		PORTB = ~i;
		_delay_ms(1);
		
		for (int j=16,r=0;j<=128;j*=2,r++)
		{
			if ((PINB & 0xF0) == ((~j) & 0xF0))
			{
				while((PINB & 0xF0)==((~j) & 0xF0));
				
				return keys[r][c];
			}
		}
	}
	return -1;
}

int getPassword(){
	lcd.lcdCmd(0x86);
	int n=0,password=0 ;
	while(1){
		int k = getKey();
		{  //keypad matrix Function
			if (k>=0 && k<=9){
				
				password = 10*password + k;
				
				n++;
				lcd.print(k);
				if (n>=4){
					return password;
					n=0;
				}
			}
		}
	}
}

ISR(INT0_vect){
	PORTC = 0x02;
	_delay_ms(1000);
	PORTC=0;
}


int main(void)
{
	DDRA = 0xFF;
	uart.begin();
	lcd.Begin();
	lcd.lcdCmd(0x01);
	lcd.print("Welcome to Toll");
	char unique[12] = {'0','2','0','0','1','0','A','0','4','2','F','0'}; //"020010A042F0";//
	int unique_pin=1111;
	int flag =0;
	DDRC = 0x03;
	
	int count = 0;
	DDRD = 0x00;
	PORTD |=(1<<PD2);
 	GICR = (1<<INT0);
 	MCUCR = (1<<ISC00)|(1<<ISC01);
	sei();
	
	while (1)
	{
		if (r==1)
		{
			r=0;
			int m = match(tag,unique);
			_delay_ms(500);
			lcd.print("match");
			if (m==1)
			{
				lcd.lcdCmd(0x01);
				lcd.lcdCmd(0x86);
				lcd.print("Enter pin:");
				_delay_ms(2000);
				
				reEnter:
				lcd.lcdCmd(0x01);
				int pswd = getPassword();
				int n=match_pin(pswd,unique_pin);
				
				if (n==1)
				{
					
					lcd.lcdCmd(0x01);
					lcd.print("THANK YOU!");
					OCR0 =200;
					PORTC = 0x01;
					_delay_ms(1000);
					PORTC = 0;
// 					while((PIND & 0x04)==0x00);
// 					PORTC = 0x01;
// 					_delay_ms(1000);
// 					PORTC=0x00;
							
// 					
// 					for (int t=0; t<6000;t++){
// 						if ((PIND & 0x04)==0x04){
// 							while(((PIND & 0x04) == 0x04));
// 							count++;
// 							OCR0 =200;
// 							PORTC = 0x01;
// 							_delay_ms(1000);
// 							PORTC=0x00;
// 							_delay_ms(1000);
// 							break;
// 						}
// 						_delay_ms(1);
// 					}
// 					
					
					
					// 					if ((PIND & 0x04) == 0x04)
					// 					{
					// 						 /*	while ((PIND & 0x04) == 0x04);
					// 							if ((PIND & 0x04) ==0x00)*/
					// 								{
					// 									count++;
					// 									OCR0 =200;
					// 									PORTC = 0x01;
					// 									_delay_ms(200);
					// 									PORTC=0x00;
					// 								}
					// 					}
					//
					// 					if ((PIND & 0x04) ==0x04 )
					// 					{
					// 						while((PIND & 0x04)==0x00);
					//
					// 						if ((PIND & 0x04) == 0x04)
					// 						{
					// 						 /*	while ((PIND & 0x04) == 0x04);
					// 							if ((PIND & 0x04) ==0x00)*/
					// 								{
					// 									count++;
					// 									OCR0 =200;
					// 									PORTC = 0x01;
					// 									_delay_ms(200);
					// 									PORTC=0x00;
					// 								 }
					// 						}
					// 					}
				}
				else
				{
					flag++;
					lcd.lcdCmd(0x01);
					//lcd.lcdCmd(0x00);
					lcd.print("Wrong Password");
					
					_delay_ms(1500);
					if (flag<=3)
					{
						lcd.lcdCmd(0xC0);
						lcd.print("Enter Again");
						_delay_ms(1500);
						goto reEnter;
						
					}
					else break;
					
				}
			}
		}
	}
}

