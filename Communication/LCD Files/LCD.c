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



	#include "LCD.h"
	#include <util/delay.h>
	#include <stdlib.h>
	
	
	
	#define SETBIT(R,B)			(R |= (1<<(B)))
	#define CLEARBIT(R,B)		(R &=~ (1<<(B)))
	#define TOGGLEBIT(R,B)		(R ^= (1<<(B)))
	#define ISBITSET(R,B)		( (R & (1<<(B))) >>(B))
	#define ISBETCLEAR(R,B)		(!ISBITSET(R,B))
	
	void LCD_init(void)
	{
		/* Set The Control Pins Direction to OUtput */
		LCD_Control_reg_DIR |=(1<<E) | (1<<RW) | (1<<RS);
		
		#if eight_bitdata
			/* Set the Direction of the 8 Data pins to Output */
			LCD_Data_Reg_DIR =0xFF;
		#else
			
			#ifdef Upper_Data_port
				/* Set The Direction of The Upper Pins to OP*/
				LCD_Data_Reg_DIR |=(0xF0);
			#else
				/* Set The Direction of The LOWER Pins to OP*/
				LCD_Data_Reg_DIR|=(0x0F);
			#endif
			Send_instruction(Return_Home);
		#endif
		
		Send_instruction(Data_Set);
		Send_instruction(Display_on);
		Send_instruction(Clear_display);
		_delay_ms(10);
	}
	
	void Send_instruction(uint8_t instruction)
	{
		/* RW, RS Should be 0 when Writing instructions to LCD */
		CLEARBIT(LCD_Control_reg_OP,RS);
		CLEARBIT(LCD_Control_reg_OP,RW);
		/* Delay of Address Set 60ns minimum */
		_delay_ms(1);
		
		/* Enable LCD E=1 */
		SETBIT(LCD_Control_reg_OP,E);
		/* delay for processing PW min 450ns */
		_delay_ms(1); 
		
		#if eight_bitdata
		
			LCD_Data_Reg_OP = instruction;
			/* Data set-up time delay  Tdsw = 195 ns */
			_delay_ms(1); 
			/* disable LCD E=0 */
			CLEARBIT(LCD_Control_reg_OP,E);
			/* Data Hold delay Th = 10ns */ 
			_delay_ms(1); 
		#else 
			
			#ifdef Upper_Data_port
				/* clear the Prev Data */
				LCD_Data_Reg_OP &=0x0F;
				LCD_Data_Reg_OP |= (instruction & 0xF0);
			
			#else	/* Lower Port */
				/* clear the Prev Data */
				LCD_Data_Reg_OP &=0xF0;
				LCD_Data_Reg_OP |= ((instruction >> 4) & 0x0f);
			#endif
			/* Data set-up time delay  Tdsw = 195 ns */
			_delay_ms(1);
		
			/* disable LCD E=0 */
			CLEARBIT(LCD_Control_reg_OP,E);
			/* Data Hold delay Th = 10ns */
			_delay_ms(1);
			
			/* Enable LCD E=1 */
			SETBIT(LCD_Control_reg_OP,E);
			/* delay for processing PW min 450ns */
			_delay_ms(1);
			#ifdef Upper_Data_port
				/* clear the Prev Data */
				LCD_Data_Reg_OP &=0x0F;
				LCD_Data_Reg_OP |= ((instruction <<4) & 0xF0);
			
			#else	/* Lower Port */
				/* clear the Prev Data */
				LCD_Data_Reg_OP &=0xF0;
				LCD_Data_Reg_OP |= ((instruction) & 0x0f);
			#endif
				/* Data set-up time delay  Tdsw = 195 ns */
				_delay_ms(1);
				/* disable LCD E=0 */
				CLEARBIT(LCD_Control_reg_OP,E);
				/* Data Hold delay Th = 10ns */
				_delay_ms(1);
		#endif
	}
	
	
	void LCD_DisplayChar(uint8_t Data)
	{
		/* RW  Should be 0  and RS should be 1 when Writing Data to LCD */
		SETBIT(LCD_Control_reg_OP,RS);
		CLEARBIT(LCD_Control_reg_OP,RW);
		/* Delay of Address Set 60ns minimum */
		_delay_ms(1);
		/* Enable LCD E=1 */
		SETBIT(LCD_Control_reg_OP,E);
		/* delay for processing PW min 450ns */
		_delay_ms(1);
		#if eight_bitdata
			LCD_Data_Reg_OP=Data;
		#else
			#ifdef Upper_Data_port
				/* clear the Prev Data */
				LCD_Data_Reg_OP &=0x0F;
				/* Send MSB (4-bits )*/
				LCD_Data_Reg_OP |= (Data & 0xF0);
				
			#else /* Lower Port */
				/* clear the Prev Data */
				LCD_Data_Reg_OP &=0xF0;
				/* Send MSB (4-bits )*/
				LCD_Data_Reg_OP |= ((Data >> 4) & 0x0f);
			#endif
				
			/* Data set-up time delay  Tdsw = 195 ns */
			_delay_ms(1);
			/* disable LCD E=0 */
			CLEARBIT(LCD_Control_reg_OP,E);
			/* Data Hold delay Th = 10ns */
			_delay_ms(1);
			/* Enable LCD E=1 */
			SETBIT(LCD_Control_reg_OP,E); 
				
			#ifdef Upper_Data_port
				/* clear the Prev Data */
				LCD_Data_Reg_OP &=0x0F;
				LCD_Data_Reg_OP |= ((Data <<4) & 0xF0);
				/* delay for processing PW min 450ns */
				_delay_ms(1);
			#else	/* Lower Port */
				/* clear the Prev Data */
				LCD_Data_Reg_OP &=0xF0;
				LCD_Data_Reg_OP |= ((Data) & 0x0f);
			#endif
			
			#endif
		/* Data set-up time delay  Tdsw = 195 ns */
		_delay_ms(1);
		/* disable LCD E=0 */
		CLEARBIT(LCD_Control_reg_OP,E);						
		/* Data Hold delay Th = 10ns */
		_delay_ms(1);
		
	}
	
	void LCD_Clear(void)
	{
		/* Clear Screen */
		Send_instruction(Clear_display); 
	}
	
	
	void LCD_DisplayString(char * ptr)
	{
		while(*ptr != '\0')
		{
			LCD_DisplayChar(*ptr);
			ptr++;
		}
	}
	
	void LCD_Select_RowCol(uint8_t Row , uint8_t col)
	{
		uint8_t Address=0;
		
		switch(Row)
		{
			case 0:
					/* When Writing To LCD 1st Col The Base Address is 0x00 */
						Address=col | LCD_1ST_BASE;
						break;
			case 1:
					/* When Writing To LCD 2nd Col The Base Address is 0x40 */
						Address=col | LCD_2ND_BASE;
						break;
			case 2:
					/* When Writing To LCD 1st Col The Base Address is 0x10 */
						Address = col | LCD_3RD_BASE;
						break;
			case 3:
					/* When Writing To LCD 1st Col The Base Address is 0x50 */
						Address=col| LCD_4TH_BASE;
						break;		
		}
		
		Send_instruction(DDRAM_CONST|Address);
	}
	
	
	void LCD_DisplayInt(int32_t data)
	{
		/* String to hold the ascii result */
		char Result[17]; 
		 /* 10 for decimal */
		itoa(data,Result,10);
		/* Display The Result */
		LCD_DisplayString(Result);
	}
	
	