/*
 * LoggingSystem.c
 *
 * Created: 4/21/2020 3:02:34 AM
 *  Author: Hamdy
 */ 

	#include "uc_Configuration.h"
	#include "MicroChip_EEPROM_25AA010A.h"
	#include "ATmega32A_UART.h"
	#include <util/delay.h>
	#define NEWLINE			(0x0D)
	void DATALogger_Init(void)
	{
		UART0.EnableMode = USART_Tx_Rx;
		UART0.CommunicationMode = USART_Async_DoubleSpeed;
		UART0.CommunicationTerminal = USART_SingleProcessor;
		UART0.InterruptSource = USART_InterruptDisable;
		UART0.DataFrame = USART_8bitData;
		UART0.ControlFrame = USART_ParityDisable_1Stop;
		UART0.BaudRate = 9600;
		USART_Init();
		EEPROM_Init();
	}
	
	void DATALogger_Start(void)
	{
		char Operation = 0;
		char Address = 0;
		char Data = 0;
		USART_SendString_Blocking("Please Enter W to Write Data to EEPROM or R to Read Data From EEPROM: ");
		USART_SendByte_Blocking(NEWLINE);
		Operation = USART_ReadByte_Blocking();
		USART_SendByte_Blocking(NEWLINE);
		switch(Operation)
		{
			case 'W':
			case 'w':
			USART_SendString_Blocking("Please Provide Address [0 - 127] Followed By Data[0-255] : ");
			
			/* Code to Convert rx String to Number */
			Address = USART_ReadByte_Blocking();
			USART_SendByte_Blocking('-');
			Data	= USART_ReadByte_Blocking();
			USART_SendByte_Blocking(NEWLINE);
			
			Address = Address - '0';
			Data	-= '0';
			EEPROM_WriteByte(Data,Address);
			USART_SendString_Blocking("Write Operation Done :: ");
			USART_SendByte_Blocking(NEWLINE);
			break;
			case 'R':
			case 'r':
			USART_SendString_Blocking("Please Provide Address [0 - 127]: ");
			Address = USART_ReadByte_Blocking();
			USART_SendByte_Blocking(NEWLINE);
			Address = Address - '0';
			Data = EEPROM_ReadByte(Address);
			
			/* Code to Convert Number to ASCII Before Printing */
			Data += '0';
			USART_SendString_Blocking("Address has Data of :  ");
			USART_SendByte_Blocking(Data);
			USART_SendByte_Blocking(NEWLINE);
			break;
			default:
			USART_SendString_Blocking("Wrong Input Please Try Again ::");
			USART_SendByte_Blocking(NEWLINE);
			break;
		}
	}