#define F_CPU 8000000UL
#define BVAD 9600UL
#define PS ((F_CPU/(16*BVAD))-1)

class serialcom{
	public:
	void begin()
	{
		UBRRL=PS;
		UBRRH=(PS>>8);
		UCSRB=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
		UCSRC=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
	}
	void write(char ch)
	{
		while(!(UCSRA&(1<<UDRE)));
		UDR=ch;
		
	}
	
	int available(){
		if (UCSRA & (1<<RXC))
			return 1;
		else
			return 0;
	}
	
	char read()
	{
		return UDR;
	}
	void print(char str[]){
		for (int i=0;str[i]!='\0';i++)
		{
			write(str[i]);
		}
		write(0x0D);
	   }
	   
	};