/*
 * LCD.h
 *
 * Created: 12/13/2017 1:46:01 AM
 *  Author: Hamdy
 *
 ******************************* internal Structure **************************
 *
 *	The LCD Contains Two 8-bit Registers ( Instruction Register for Storing Instructions ) (Data Register That Stores incoming Data).
 *	Character Generator ROM (CGROM)   ( Contains Predefined 5 * 8 (208 Character) dot or 5 * 10 (32 Char) dot character patterns.
 *  Character Display RAM (DDRAM)   (In the character generator RAM, the user can rewrite character patterns by program. For 5 * 8 dots.)
 *
 *
 ******************************* Operating Modes *****************************
 *
 * (8-bit Mode)			all eight bus lines (DB0 to DB7) are used.
 * (4-bit Mode)			four bus lines (DB4 to DB7) are used for transfer to transfer data, the four high order bits of Data, are transferred before the four low order bits.
 *
 *
 ****************************** pinout Functions *****************************
 *
 *										* Control PINS *
 *	RS					Select  0 for Instruction Register ,  1 for Data Register 
 *	RW				    Select  0 for Write , 1 For Read
 *  E				    Starts Data read/Write ( Transaction From High To Low When Writing , Transaction From Low to High when Reading Data will be Available as long as the signal is High)
 *
 *										* Data PINS *
 *  DB4 to DB7			Four high order bidirectional pins. Used for data transfer and receive between the MPU and the LCD.
 *	DB0 to DB3			Four Lower order bidirectional pins. Used for data transfer and receive between the MPU and the LCD.
 *  DB7					Can Be Used As state indicator Flag
 *
 *										* POWER PINS *
 *	VSS					Power Supply pin should be connected to GND
 *  VDD					Power Supply pin should be connected to VCC ( 3.3 ~ 9V)
 *  VO					Power Supply pin controls the LCD Contrast 
 *	A					The Anode Terminal of the back LCD (Should be Driven High 3V)
 *	K					The cathode Terminal of The Back of the LCD ( GND )
 *
 *
 ******************************** Function Description **************************
 * 
 * Busy Flag (BF)   When the busy flag is 1 The LCD is Busy And Cannot Take Instructions You Can Check The Busy Flag When RS=0,RW=1 @ DB7
 *
 *								Operation Selection
 *				RS				R/W				Operation
 *				0				0			    IR write as an internal operation (display clear, etc.)
 *				0				1				Read busy flag (DB7) and address counter (DB0 to DB6)
 *				1				0				Write To LCD
 *				1				1				Read The Contents of The LCD
 *
 *
 ******************************** Instructions ************************************
 *
 *	Clear display							Clears entire display and sets DDRAM address 0 in address counter.
 *	Return home				 (1.52ms)		Sets DDRAM address 0 in address counter. Also returns display from being shifted to original position DDRAM contents remain unchanged.
 *	Entry mode set			 (37 us )		Sets cursor move direction	and specifies display shift.
 *	Display on/off control	 (37 us )		Controls LCD , Cursor , Blinking.
 *  Cursor or display shift	 (37 us )		shifts LCD without changing it's Content.
 *  Function set			 (37 us )		Sets interface data length (DL),number of display lines(N),and character font (F).
 *	Read busy flag & address (0 us )	    Reads busy flag (BF)
 *	Set DDRAM address		 (37 us )
 *	Set CGRAM address		 (37 us )
 *
 *
 *
 *
 */ 


#ifndef LCD_H_
#define LCD_H_
	
#include "uc_Configuration.h"
#include <avr/io.h>
#include <stdint.h>

		/* HW Connections */
#define  RS   PIN0
#define  RW   PIN1
#define  E    PIN2

#define LCD_Control_reg_DIR			DDRA
#define LCD_Control_reg_OP			PORTA
#define LCD_Control_reg_IP			PINA

#define LCD_Data_Reg_DIR			DDRA
#define LCD_Data_Reg_OP				PORTA
#define LCD_Data_Reg_IP				PINA



/* Define Upper Data to Use The Higher 4 Pins PD4 --> PD7 */
#define Upper_Data_port 1
		
		/* init Settings */
#define CURSOR  1		/* 1 to Enable , 0 to Disable */
#define Blink	1	/* 1 to Enable , 0 to Disable */
#define eight_bitdata 0 /* 1 to Enable , 0 for 4 bit */
#define _5x10_Dot	 0	/* 1 to Enable , 0 for 5 x 8 Dot */
#define Two_lines	 1/* 1 to Enable , 0 for 1 Line */


		#if ( _5x10_Dot && Two_lines )
				#pragma GCC error "You cannot Have Two Lines LCD with 5 * 10"
		#endif
		
		/* List Of Command Codes */
#define Clear_display 0x01
#define Return_Home	  0x02
#define Display_off   0x08
#define Display_on   (0x0C | (CURSOR<<1) | (Blink))
#define CURSOR_Shift_Right	 0x14
#define CURSOR_Shift_left	 0x10
#define Display_shift_left    0x18
#define Display_shift_Right	  0x1C
#define Data_Set (0x20 |(_5x10_Dot<<2)|(Two_lines<<3)| (eight_bitdata<<4))
#define DDRAM_CONST 0x80

#define LCD_1ST_BASE 0x00
#define LCD_2ND_BASE 0x40
#define LCD_3RD_BASE 0x10
#define LCD_4TH_BASE 0x50

#define LCD_LEFT 1
#define LCD_RIGHT 0

#define LCD_Shift(DIR)	((DIR == LCD_LEFT)? Send_instruction(Display_shift_left) : Send_instruction(Display_shift_Right))
#define LCD_CURSOR_Shift(DIR) ((DIR == LCD_LEFT)? Send_instruction(CURSOR_Shift_left) : Send_instruction(CURSOR_Shift_Right))

void Send_instruction(uint8_t instruction);
void LCD_init(void);
void LCD_Clear(void);
void LCD_DisplayChar(uint8_t Data);
void LCD_DisplayString(char * ptr);
void LCD_Select_RowCol(uint8_t Row , uint8_t col);
void LCD_DisplayInt(int32_t data);

#endif /* LCD_H_ */