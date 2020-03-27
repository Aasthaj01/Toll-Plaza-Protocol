#define LCD PORTA
class LiquidCrystal{
	
	public: void lcdCmd(int cmd){
		LCD = ((cmd & 0xF0)/4)+2;
		_delay_ms(2);
		LCD-=2;
		
		LCD = ((cmd & 0x0F)*4)+2;
		_delay_ms(2);
		LCD-=2;
	}
	
	public:void data(int data){
		LCD = ((data & 0xF0)/4)+3;
			_delay_ms(2);
			LCD-=2;
			
			LCD = ((data & 0x0F)*4)+3;
			_delay_ms(2);
			LCD-=2;
	}
	public:void Begin()
	{
		lcdCmd(0x02);
		lcdCmd(0x28);
		lcdCmd(0x0C);
		lcdCmd(0x06);
		lcdCmd(0x01);
	}
	void print(int key){
		if (key<10)
		{
			data(key+48);
		}
		else{
			print(key/10);
			
			data((key%10) + 48);
		}
	}
	
	void print(char c[])
	{
		for (int i=0;c[i]!='\0';i++)
		{
			data(c[i]);
		}
	}
public:void clear()
	{
		lcdCmd(0x01);
	}
	
};