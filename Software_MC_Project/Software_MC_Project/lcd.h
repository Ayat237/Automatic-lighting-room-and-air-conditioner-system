/**************************
 *
 * Module: LCD
 *
 * File Name: lcd.h
 *
 * Description: Header file for the LCD driver
 *
 * Author: Mohamed Hassan
 *
 ***************************/
#ifndef LCD_FILES
#define LCD_FILES
#include"std_types.h"
/* LCD HW Ports and Pins Ids */
#define LCD_RS_PORT_ID PORTA_ID
#define LCD_RS_PIN_ID PIN1_ID
#define LCD_ENABLE_PORT_ID PORTA_ID
#define LCD_ENABLE_PIN_ID PIN2_ID
#define LCD_DATA_PATH_REG PORTA_ID
#define LCD_MODE 4
#if(LCD_MODE==4)
#define LCD_DB4_ID PIN3_ID
#define LCD_DB5_ID PIN4_ID
#define LCD_DB6_ID PIN5_ID
#define LCD_DB7_ID PIN6_ID
#endif
/* LCD Commands */
#define LCD_TWO_LINE_EIGHT_BIT_MODE 0x38
#define LCD_ONE_LINE_EIGHT_BIT_MODE 0x30
#define LCD_ONE_LINE_FOUR_BIT_MODE 0x20
#define LCD_TWO_LINE_FOUR_BIT_MODE 0x28
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_CURSOR_OFF   0x0C
#define LCD_CURSOR_ON 0x0E
#define LCD_CURSOR_POSITION 0x80
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1   0x33
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2   0x32


/***************************
 *                      Functions Prototypes                                   *
 ***************************/
/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_init(void);
/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(unsigned char command);
/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayChar(unsigned char data);
/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(unsigned char * LCD_data_ptr);
/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(unsigned char row,unsigned char column);
/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displayStringRowColumn(unsigned char row,unsigned char column,const char*Str);
/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_intgerToString(int data);
/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void);
#endif