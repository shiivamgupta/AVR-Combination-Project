/*lcd_usart_ext interrupt_timer_gpio_adc
 * LCD_USART_EXTINT.c
 *
 * Created: 15-09-2019 05:51:25
 * Author : DESD
 */ 
#define F_CPU 16000000										//CPU Frequency is set 16Mhz
#include <avr/io.h>
#include<avr/interrupt.h>									//header file for interrupt
#include<stdlib.h>											

void lcd_in();
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_str(char *);
void clear_bit(unsigned	int);
void set_bit(unsigned int);
void enable();
void lcd_goto(unsigned int,unsigned int);
void delay(unsigned int);
char s[20];float b;
char h[10];

int a;
unsigned int count;
int i;




void delay(unsigned int x)					//delay function x is in ms of delay 
{
	TCNT1=0XC180;							//counter lower value set
	TCCR1A=0;								//timer counter control register 1A 
	
	TCCR1B=0X01;							//(1<<cs10)for no prescaling
	for(int j=0;j<x;j++)					//for loop as once counter can generate only i ms second so for xms 
	{
		while(((TIFR)&(1<<TOV1))!=0x04);	//as if counter overflows then tov1 bit of tifr will be set
		TCNT1=0xc180;						// as after overflowing tcnt1 value will be 0 so we have to reinitiate
		TIFR=0x04;							//as we can reset this flag by writing 1 to it
	}
}


ISR(USART1_RX_vect)							//recieve interrupt service routine 
{	PORTA|=(1<<3);							//for buzzer
	
	PORTC=~PORTC;							//toggles led
	s[i]=UDR1;								//character recieved through tera term will be store in this char array
	i++;									//incrementing array index value
	
}


ISR(INT6_vect)								//interrupt service routine for int6 connected to switch 3
{

	PORTA|=(1<<3);							//for buzzer
	PORTC=0X00;								//switch on led
	lcd_cmd(0x01);							//clear display
	delay(1000);							//1sec or 1000 ms delay
	for(int j=0;j<i;j++)
	{
		lcd_data(s[j]);							//printing data recieved from usart
		delay(10000);
	}
								//delay for 10 sec or 10000 ms 
	PORTC=0Xff;								//led off
	i=0;									//reinititating array index value to 0
}
	

int main(void)								//main function
{	
	DDRA=0xff;								//PORT A becomes output port 
	DDRC=0XFF;								//PORT C becomes output port	
	DDRD=0X00;								//PORT D becomes input port
	DDRE=0X00;								//PORT E  becomes input port
	PORTE=0xff;									
	
	UBRR1H=0;
	UBRR1L=103;								//setting baudrate
	UCSR1A=0;
	UCSR1B=0X98;							//(1<<rxcie)|(1<<rxen)|(!<<txen)
	UCSR1C=0X06;							//(1<<ucsz11)|(1<<ucsz10)

	
	
	lcd_in();								//2 lines 5x7 dots
	lcd_cmd(0x14);							//shifts cursor to right 
	lcd_cmd(0x06);							//increments in ddram
	lcd_cmd(0x0e);							//display is cursor is on 
	lcd_cmd(0x01);							//clears the display 
	lcd_cmd(0x80);							//gives starts address to ddram of lcd 
	lcd_str("ENTER REQ DATA");
	delay(5000);							//delay of 5 second
	lcd_cmd(0x01);							//clears display of lcd
	lcd_str("USE Switch 3");
	
	EICRA=0x00;								
	EICRB=0x10;								//(1<<ISC60) to give on logical change
	EIMSK=0x40;								//to enable int6 interrupt
	
	sei();									//set enable globally

    while (1)								//infinite loop
    {
    }
}

void lcd_in()
{	
	lcd_cmd(0x28);
	clear_bit(0);							//clears zeroth bit
	enable();
	
}
void lcd_cmd(unsigned char c)
{
	PORTA=(c&0xf0);							//sending upper bit to port A
	clear_bit(0);
	enable();
	c=(c<<4);								//shifts c 4 times
	PORTA=(c&0xf0);							//sends lower 4bit
	clear_bit(0);
	enable();
}
void lcd_data(unsigned char c)
{
	PORTA=(c&0xf0);							//sending upper bit to port A
	set_bit(0);
	enable();
	c=(c<<4);								//shifts c 4 times
	PORTA=(c&0xf0);							//sends lower 4bit
	set_bit(0);
	enable();
}
void lcd_str(char *z)
{
	while(*z!='\0')
	{
		lcd_data(*z++);						//prints whole string
		
	}
}
void clear_bit(unsigned	int a)
{
	PORTA &=(~(1<<a));						//bit position given is cleared
}
void set_bit(unsigned int a)
{
	PORTA |=(1<<a);							//bit position given is set 
}
void enable()
{
	set_bit(2);		
	delay(20);
	clear_bit(2);
	delay(20);
}

void lcd_goto(unsigned int x,unsigned int y)
{
	if(x==0)
		lcd_cmd(0x80|y);					//y is coordinates of first line
	if(x==1)
		lcd_cmd(0xc0|y);					//y is coordinates of second line
}
