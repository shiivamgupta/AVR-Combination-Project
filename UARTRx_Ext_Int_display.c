/*
 * UARTRx_extInt_display_lcd.c
 *
 * Created: 15-09-2019 08:24:18
 * Author : DESD
 */ 
#define F_CPU 16000000

#define lcd PORTA
#define En 2
#define rs 0

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

void uart_string(unsigned char *);
void uart_tx(unsigned char);
char uart_rx();
void uart_i();

void lcd_i();
void lcd_CMD(unsigned int);
void lcd_DATA(unsigned int);
void en();
void lcd_str(char *);

static char a[30];

int main(void)
{
	DDRC=0xff;
	
	lcd_i();
    uart_i();
	EIMSK|=(1<<INT6);
	sei();
	
	
	
	
	static int i=0;
	
    while (1) 
    {
		a[i]=uart_rx();
		i++;
		
    }
}



ISR(INT6_vect){
	cli();
	 lcd_str(a);
	_delay_ms(500);
	sei();
}


void uart_i(){
	
	UBRR1L=0x67;			// setting 9600 baud rate
	UCSR1B=0x18;
	UCSR1C|=(1<<UCSZ10)|(1<<UCSZ11);
}



char uart_rx(){
	
	while(!(UCSR1A&(1<<RXC1)));
	UCSR1A|=(1<<RXC1);
	return UDR1;
}



void uart_string(unsigned char *str){
	for(int i=0;str[i]!='\0';i++){
		uart_tx(str[i]);
		_delay_ms(40);
	}
}

void lcd_CMD(unsigned int cmd){
	
	lcd =(cmd & 0xf0);
	lcd &=~(1<<rs);
	en();
	lcd =(cmd<<4);
	lcd &=~(1<<rs);
	en();
}

void lcd_DATA(unsigned int data){
	
	lcd=(data &(0xf0))|(1<<rs);
	en();
	lcd=(data<<4)|(1<<rs);
	en();	
}

void en(){
	lcd|=(1<<En);
	_delay_ms(10);
	lcd&=~(1<<En);
	_delay_ms(10);	
}


void lcd_i(){
	_delay_ms(50);
	DDRA=0b11110101;
	PORTC=0x0f;
	lcd_CMD(0x02);
	lcd_CMD(0x28);
	lcd_CMD(0x01);	
	lcd_CMD(0x0f);
	lcd_CMD(0x80);	
}



void lcd_str(char *str){
	
	for(int i=0;str[i]!='\0';i++){
		lcd_DATA(str[i]);
		_delay_ms(10);
	}

}
