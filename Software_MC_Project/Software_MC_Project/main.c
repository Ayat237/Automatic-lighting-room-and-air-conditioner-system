/*
   Project_name : Automatic Room Lighting System with temperature sensor to control the air conditioner
   Author : -Ayat mohamed mohamed 
            -Alaa read saed
			-bassant maher 
			-toqa yasser
			-Ahmed ali fahmy
			-ahmed hussien badr
			-loay tamer  
*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "common_macros.h"
#include "lcd.h"

unsigned int x = 0;

void speed(void)
{
	if (x == 0)
	{
		//max value of CTC(clear timer on compare match) timer
		OCR0 = 0; // (0/255)*100 = 0 duty cycle (stop fan)
	}
	else if (x == 1)
	{
		//max value of CTC(clear timer on compare match) timer
		OCR0 = 84; // (84/255)*100 =33 duty cycle
	}
	else if (x == 2)
	{
		//max value of CTC(clear timer on compare match) timer
		OCR0 = 192; //(192/255)*100 = 75 duty cycle
	}
	else
	{
		//max value of CTC(clear timer on compare match) timer
		OCR0 = 255; // (255/255)*100 = 100 duty cycle
	}
}

void adc_init()
{
	// SELECT 128 PreScaler
	SET_BIT(ADCSRA, ADPS0);
	SET_BIT(ADCSRA, ADPS1);
	SET_BIT(ADCSRA, ADPS2);
	// Enable ADC
	SET_BIT(ADCSRA, ADEN);
}

uint16_t read_adc(uint8_t ch)
{
	// Select Vref = 2.56V
	ADMUX = 0xC0;
	// Select Channel
	ADMUX |= ch;
	// Start Conversation and return the value
	SET_BIT(ADCSRA, ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

int main()
{
	// Initialization
	LCD_init();
	adc_init();
	int flag = 0;
	int count = 0;
	int temp_count;
	SET_BIT(DDRC, 4);
	CLEAR_BIT(DDRD, 0);
	CLEAR_BIT(DDRD, 1);
	DDRB |= (1 << PB3);
	// For Motor
	OCR0 = 0;	
	// ( WGM00 , WGM01 ) for Fast PWM mode and ( COM01 ) Clear OC0 on compare match , set OC0 at TOP
	// when WGM00 = 1 , WGM01 = 1 then Fast PWM --> Compare Output Mode waveform generator on OC0 pin									
	TCCR0 |= (1 << COM01) | (1 << WGM00) | (1 << WGM01);
	// PreScaler 8 (CS02,CS01,CS00 =010)
	TCCR0 |= (1 << CS01);						  	   
	uint16_t digital_temp;
	float analog_temp = 0.0;
	float temp_value = 0.0;
	
	while (1)
	{
		// Configuration 
		// take digital value of temp from ADC
		digital_temp = read_adc(0);
		
		//convert digital temp to analog
		temp_value = analog_temp;
		analog_temp = ((digital_temp * 2.56) / 1023) * 100;
		
		//configuration of PWM speed
		speed();
		
		temp_count = count;
		
		// first IR sensor check if there is a person enter the room or not 
		if(PIND & (1<<0))
		{
			_delay_ms(100);
			// 2end IR sensor
			if(PIND & (1<<1))
			{
				count += 1;
				flag = 1;
				PORTC |= (1<<4);
			}
			while(PIND & (1<<1));
		}
		else if(PIND & (1<<1))
		{
			_delay_ms(100);
			if(PIND & (1<<0))
			{
				if(count > 0){
					count -=1;
					flag = 1;
				}
				if(count == 0){
					flag = 0;
					PORTC &= ~(1<<4);
				}
			}
			while(PIND & (1<<0));	
		}
		if (temp_value != analog_temp || temp_count != count)
		{
			LCD_clearScreen();
			LCD_displayString(" Temperature:");
			LCD_intgerToString(analog_temp);
			LCD_sendCommand(0xC0);
			LCD_displayString(" Count:");
			LCD_intgerToString(count);
		}
		
		if (flag == 1)
		{
			if (analog_temp < 20)
			{
				x = 0;// 0 % duty cycle 
			}
			else if (analog_temp >= 20 && analog_temp < 25)
			{
				x = 1;// 33 % duty cycle 
			}
			else if (analog_temp >= 25 && analog_temp < 35)
			{
				x = 2;// 75% duty cycle 
			}
			else if (analog_temp >= 35)
			{
				x = 3; // 100% duty cycle 
			}
		}else
		{
			x = 0;	
		}
	}
}