/**************************
 *
 * Module: LCD
 *
 * File Name: lcd.c
 *
 * Description: Source file for the LCD driver
 *
 * Author: Mohamed Hassan
 *
 ***************************/
#include "lcd.h"
#include <util/delay.h> /* For the delay functions */
#include "common_macros.h" /* For GET_BIT Macro */
#include "gpio.h"

/***************************
 *                      Functions Definitions                                  *
 ***************************/

/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 8-bits.
 */
void LCD_init(void){
	/* Configure the direction for RS and E pins as output pins */
	GPIO_setupPinDirection(LCD_RS_PORT_ID,LCD_RS_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_ENABLE_PORT_ID,LCD_ENABLE_PIN_ID,PIN_OUTPUT);

	_delay_ms(20); /* LCD Power ON delay always > 15ms */
#if(LCD_MODE ==8)
	/* Configure the data port as output port */
	GPIO_setupPortDirection(LCD_DATA_PATH_REG,PORT_OUTPUT);
	/* use 2-lines LCD + 8-bits Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(LCD_TWO_LINE_EIGHT_BIT_MODE);

#elif(LCD_MODE==4)
	/* Configure the data pins as output pins */
	GPIO_setupPinDirection(LCD_DATA_PATH_REG,LCD_DB7_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PATH_REG,LCD_DB6_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PATH_REG,LCD_DB5_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PATH_REG,LCD_DB4_ID,PIN_OUTPUT);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2 );
	/* use 2-lines LCD + 4-bits Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(LCD_TWO_LINE_FOUR_BIT_MODE);
#endif

	 /* cursor off */
	LCD_sendCommand( LCD_CURSOR_OFF );
	/* clear LCD at the beginning */
	LCD_sendCommand(LCD_CLEAR_DISPLAY );
}

/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(unsigned char command){
	 /* Instruction Mode RS=0 */
	GPIO_writePin(LCD_RS_PORT_ID,LCD_RS_PIN_ID,LOGIC_LOW);
	_delay_ms(1); /* delay for processing Tas = 50ns */
	GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID,LOGIC_HIGH); /* Enable LCD E=1 */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
#if(LCD_MODE ==8)
	GPIO_writePort(LCD_DATA_PATH_REG, command); /* out the required command to the data bus D0 --> D7 */
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID,LOGIC_LOW) /* Disable LCD E=0 */;
	_delay_ms(1); /* delay for processing Th = 13ns */

#elif(LCD_MODE==4)
	GPIO_writePin(LCD_DATA_PATH_REG,LCD_DB4_ID,GET_BIT(command,4));
	GPIO_writePin(LCD_DATA_PATH_REG,LCD_DB5_ID,GET_BIT(command,5));
	GPIO_writePin(LCD_DATA_PATH_REG,LCD_DB6_ID,GET_BIT(command,6));
	GPIO_writePin(LCD_DATA_PATH_REG,LCD_DB7_ID,GET_BIT(command,7));
	_delay_ms(1);
	GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID,LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID,LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePin(LCD_DATA_PATH_REG,LCD_DB4_ID,GET_BIT(command,0));
	GPIO_writePin(LCD_DATA_PATH_REG,LCD_DB5_ID,GET_BIT(command,1));
	GPIO_writePin(LCD_DATA_PATH_REG,LCD_DB6_ID,GET_BIT(command,2));
	GPIO_writePin(LCD_DATA_PATH_REG,LCD_DB7_ID,GET_BIT(command,3));
	_delay_ms(1);
	GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID,LOGIC_LOW);
	_delay_ms(1);

#endif

}

void LCD_displayChar(unsigned char data){
	GPIO_writePin(LCD_RS_PORT_ID,LCD_RS_PIN_ID,LOGIC_HIGH); /* Data Mode RS=1 */
	_delay_ms(1); /* delay for processing Tas = 50ns */
	GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID,LOGIC_HIGH);  /* Enable LCD E=1 */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
#if(LCD_MODE ==8)
	GPIO_writePort(LCD_DATA_PATH_REG, data); /* out the required data to the data bus D0 --> D7 */
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID,LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */

#elif(LCD_MODE==4)
	/* out the required data to the data bus D4 --> D7 */
	GPIO_writePin(LCD_DATA_PATH_REG,LCD_DB4_ID,GET_BIT(data,4));
	GPIO_writePin(LCD_DATA_PATH_REG,LCD_DB5_ID,GET_BIT(data,5));
	GPIO_writePin(LCD_DATA_PATH_REG,LCD_DB6_ID,GET_BIT(data,6));
	GPIO_writePin(LCD_DATA_PATH_REG,LCD_DB7_ID,GET_BIT(data,7));
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID,LOGIC_LOW);/* Disable LCD E=0 */
	_delay_ms(1);/* delay for processing Th = 13ns */
	GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID,LOGIC_HIGH);
	_delay_ms(1);
	/* out the rest of the required data to the data bus D4 --> D7 */
	GPIO_writePin(LCD_DATA_PATH_REG,LCD_DB4_ID,GET_BIT(data,0));
	GPIO_writePin(LCD_DATA_PATH_REG,LCD_DB5_ID,GET_BIT(data,1));
	GPIO_writePin(LCD_DATA_PATH_REG,LCD_DB6_ID,GET_BIT(data,2));
	GPIO_writePin(LCD_DATA_PATH_REG,LCD_DB7_ID,GET_BIT(data,3));
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_writePin(LCD_ENABLE_PORT_ID, LCD_ENABLE_PIN_ID,LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */

#endif

}
/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(unsigned char * LCD_data_ptr){
	while(*LCD_data_ptr!='\0'){
		LCD_displayChar(*LCD_data_ptr);
		LCD_data_ptr++;
	}
}

/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(unsigned char row,unsigned char column){
	unsigned char LCD_address=0;
	/* Calculate the required address in the LCD DDRAM */
	switch(row){
	case 0 :
	{
		LCD_address=column;
		break;
	}
	case 1 :
	{
		LCD_address=0x40+column;
		break;
	}
	case 2:
	{
		LCD_address=0x10+column;
		break;
	}
	case 3:
	{
		LCD_address=0x50+column;
		break;
	}
	}
	/* Move the LCD cursor to this specific address */
	LCD_sendCommand(LCD_CURSOR_POSITION|LCD_address);
}

/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displayStringRowColumn(unsigned char row,unsigned char column,const char*Str){
	LCD_moveCursor(row,column); /* go to to the required LCD position */
	LCD_displayString(Str); /* display the string */
}
/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void){
	LCD_sendCommand(LCD_CLEAR_DISPLAY);
}

/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_intgerToString(int data)
{
	char buff[16]; /* String to hold the ascii result */
	itoa(data,buff,10); /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
	LCD_displayString(buff); /* Display the string */
}