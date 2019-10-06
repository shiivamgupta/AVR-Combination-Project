/*
 * Test2.c
 *
 * Created: 15-09-2019 19:11:31
 * Author : DESD
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <string.h>
void str_trans(char *str);
void init_UART(void);
void transmit(unsigned char data);
void str_recv(void);
void mesDisp(void);
void ledOn(void);
void ledOff(void);
//void ReadTemp();
//void adc_init();
unsigned char receive (void);
static char str[50];
static char stra[]="Hello DESD";
void lcd_init()
{
	DDRA=0xff;
	lcd_cmd(0x28);
	clear_bit(0);
	enable();
	
	lcd_cmd(0x0c);
	lcd_cmd(0x01);
	lcd_cmd(0x80);
}

void lcd_cmd(unsigned char ch)
{
	PORTA=(ch & 0xf0);
	clear_bit(0);
	enable();
	ch=ch<<4;
	PORTA=(ch & 0xf0);
	clear_bit(0);
	enable();
}

void lcd_data(unsigned char ch)
{
	PORTA=(ch & 0xf0);
	set_bit(0);
	enable();
	ch=ch<<4;
	PORTA=(ch & 0xf0);
	set_bit(0);
	enable();
}

void clear_bit(unsigned int ch)
{
	PORTA=PORTA &(~(1<<ch));
}

void set_bit(unsigned int ch)
{
	PORTA=PORTA |(1<<ch);
}

void enable()
{
	set_bit(2);
	_delay_ms(20);
	clear_bit(2);
	_delay_ms(20);
}

void lcd_string(char *str)
{
	while(*str!='\0')
	{
		lcd_data(*str++);
	}
}
void init_UART(void)
{
	UBRR1H=0x00;
	UBRR1L=0x67;
	UCSR1C|=(1<<UCSZ0)|(1<<UCSZ1);
	UCSR1B|=(1<<TXEN)|(1<<RXEN);
	
}

int main(void)
{
	int i=0,j=0;
	init_UART();
	lcd_init();
	DDRC=0XFF;
	PORTC=0XFF;
	static char str1[]="Welcome to Smart Home Login";
	static char str9[]="\r\n";
	static char str2[]="Please Enter your Username";
	static char str3[]="CDAC";
	static char str4[]="Please Enter your Password";
	static char str5[]="Please Enter valid Username";
	static char str6[]="ACTS123";
	static char str7[]="Please Enter valid Password";
	static char str8[]="Please Enter Choice 1. LED ON 2. LED OFF";
	
	str_trans(str1);//Transmit first Message Welcome
	str_trans(str9);
	str_trans(str2);//Transmit Second Message To enter Username
	str_trans(str9);
	str_recv();//Receive username
	for (i=0;i<4;i++)
	{
		if(str[i]==str3[i])//Check if Username is correct
		continue;
		else break;//if wrong then break from loop
	}
	if(i==4)//if Username is correct
	{
		str_trans(str4);//Accept password from user
		str_recv();
		for (j=0;j<7;j++)
		{
			if(str[j]==str6[j])//check whether password is correct
			continue;
			else
			break;
		}
		if(j==7)//if password is correct
		{
			while(1){
				str_trans(str9);
				str_trans(str8);
				int choice = receive()-48;
				switch(choice)
				{
					case 1:
					ledOn();
					str_trans(str9);
					break;
					
					case 2:
					//ReadTemp();
					ledOff();
					str_trans(str9);
					break;
					
				}
			}
			
			
			
			
		}
		else//if password is wrong
		{
			str_trans(str7);//Display message invalid Password
		}
	}
	else
	str_trans(str5);//if username is wrong
	
	
}
/*void ReadTemp()//To read temparature
{
	adc_init();
	DDRC=0xFF;
	PORTC=0xff;
	while(1)
	{
		while(ADCSRA!=(ADCSRA|(1<<ADIF)));
		PORTC= ADCH;  //LED
		char *str2=ADCH;
		str_trans(str2);
		_delay_ms(100);
		
	}
}*/
void ledOn(void)//To initiate ledon function
{
	DDRC=0xff;
	PORTC=0x00;
	lcd_cmd(0x01);
	lcd_string(stra);
	
}
void ledOff(void)//To initiate ledon function
{
	DDRC=0xff;
	PORTC=0xff;
	lcd_cmd(0x01);
	
}
void str_recv()//To receive String Funvtion
{
	int i=0;
	//unsigned char str[10];
	char x;
	for(i=0;i<50;i++)
	{
		x=receive();
		if(x==13)
		{
			str[i]='\0';
			break;
		}
		else
		{
			str[i]=x;
		}
		
	}
	
	str[i]='\0';
	
}

void str_trans(char* str)//To transmit string
{
	int i=0;
	while(str[i]!='\0')
	{
		transmit(str[i]);
		i++;
	}
}
void transmit(unsigned char data)//To transmit char
{
	while(!(UCSR1A & (1<<UDRE1)));
	UDR1=data;
	
}
unsigned char receive(void)//to Receive Characters
{
	while(!(UCSR1A & (1<<RXC)));
	return UDR1;
}
/*void adc_init()//Initialise adc_init
{
	ADMUX= (1<<REFS0)|(1<<ADLAR);
	ADCSRA= 0xE7;


}*/

