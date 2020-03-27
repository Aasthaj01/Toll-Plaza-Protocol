/*
 * tollPlaza.cpp
 *
 * Created: 20-07-2019 09:28:58
 * Author : hp
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "lcd16x2.h"
#include "usart.h"
#include <avr/interrupt.h>

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
int match_pin(int str1[],int str2[])
{
	int m=2;
	for (int i=0;i<4;i++){
		if (str1[i]!=str2[i]){
			m=4;
		}
	}
	return m;
}

volatile int i=0; char tag[12];char pin[4];
ISR(USART_RXC_vect){
	tag[i]=UDR;
	i++;
	if (i==12){
	i=0;
	}
}

void nullTag(char tag[])
 {
	 for (i=0;i<12;i++)
	 {
		 tag[i]='\0';
	 }
 }
 
 int keys[4][4] = {{0,1,2,3},{4,5,6,7},{8,9,10,11},{12,13,14,15}};
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
int main(void)
{
	DDRA = 0xFF;
	serialcom uart;
	LiquidCrystal lcd;
	
	uart.begin();
	lcd.Begin();
	lcd.lcdCmd(0x01);
	lcd.print("Welcome to Toll");
	char unique[12] = {'0','2','0','0','1','0','A','0','4','2','F','0'}; //"020010A042F0";//	
	int comp[4] = {'0','1','0','1'};
	int flag =1;
	DDRC = 0x03;
	PORTC = 0x20;
	TCCR0 = (1<<WGM00)|(1<<WGM01)|(1<<COM01)|(1<<CS02);
	TCNT0 = 0;	
	
	int count = 0;
	DDRD = 0x00;
	DDRB = 0xFF;
		
		
   int i=0;
    while (1) 
    {
		lcd.lcdCmd(0x01);
		lcd.print("scan card");
// 		r: 
// 		if (uart.available()){
// 			tag[i] = uart.read();
// 			i++;
// 			}
// 		}
// 		if (i!=12){
// 			goto r;
// 		}
		
		
			int m = match(tag,unique);
			
			if (m==1)
			{
				lcd.lcdCmd(0x80);
				lcd.print("Enter the pin:");
				lcd.lcdCmd(0xC0);
				
				int pin[] = {};
				int i = 0;
				lcd.Begin();
				lcd.lcdCmd(0x86);
				
				while (1)
				{
					int k = getKey();
					if (k!= -1)
					{
						if (k==14)
						{
							i--;
							pin[i] = '\0';
													
							
						}
						
						else{
							
							pin[i] = k;
							i++;
							
							
						}
						
					}
					lcd.clear();
					for (int j=0;pin[j]!='\0';j++)
					{
						lcd.print("*");
					}
				}
			while(flag<=3)
			  {int mp = match_pin(pin,comp);
				 flag++;
				if (mp==2)
				{
					OCR0 =200;
					PORTC = 0x02;
					_delay_ms(200);
					PORTC = 0;
					lcd.print("THANK YOU!");
				}
				
				else if (mp==4)
				{ if (flag<=3)
				{lcd.print("Re-enter the pin:");
					lcd.lcdCmd(0xC0);
					int mp = match_pin(pin,comp);
					flag++;
				}
					else
					{lcd.print("exceeded attempt");
						break;}
				}
							
			
			}
				
			}
// 			else{
// 				lcd.print("not matched");
// 			}	
		if ((PIND & 0x30) ==0x20 )
		{
			while((PIND & 0x30)==0x20);
			if ((PIND & 0x30) == 0x30)
			{
				while ((PIND & 0x30) == 0x30);
				if ((PIND & 0x30) ==0x10)
				{
					while((PIND & 0x30)==0x10);
					
					if ((PIND & 0x30)==0x00)
					{
						count++;
					}
				}
			}
			
			
			lcd.lcdCmd(0xC5);
			lcd.print(count);
			_delay_ms(500);
		
    }
	
			
	}
	}

