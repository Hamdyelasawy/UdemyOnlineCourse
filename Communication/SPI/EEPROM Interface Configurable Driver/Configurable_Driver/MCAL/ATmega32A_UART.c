/*
 * ATmega32A_UART.c
 *
 * Created: 4/10/2020 5:30:09 AM
 *  Author: Hamdy
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "ATmega32A_UART.h"
#include "uc_Configuration.h"




USART_ConfigurationStruct  UART0 = { 0,
									.RXC_InterruptCall  = NULL_PTR,
									.TXC_InterruptCall  = NULL_PTR,
									.UDRE_InterruptCall = NULL_PTR };

ISR(USART_RXC_vect)
{
	if ( UART0.RXC_InterruptCall != NULL_PTR)
	{
		UART0.RXC_InterruptCall();
	}
	else
	{
		// Handle Error Using Error Handling Function
		
	}
}

ISR(USART_TXC_vect)
{
	if ( UART0.TXC_InterruptCall != NULL_PTR)
	{
		UART0.TXC_InterruptCall();
	}
	else
	{
		// Handle Error Using Error Handling Function
		
	}
}

ISR(USART_UDRE_vect)
{
	if ( UART0.UDRE_InterruptCall != NULL_PTR)
	{
		UART0.UDRE_InterruptCall();
	}
	else
	{
		// Handle Error Using Error Handling Function
		
	}
}


void USART_Init(void)
{
	unsigned char UCSRA_Temp=0;
	unsigned char UCSRB_Temp=0;
	unsigned char UCSRC_Temp=0;
	float UBRR_Temp=0;
		
	switch(UART0.EnableMode)
	{
		case USART_Disable:
		break;
		case USART_Rx_Only:
			 UCSRB_Temp |= (1<<RXEN);
		break;
		case USART_Tx_Only:
		     UCSRB_Temp |= (1<<TXEN);
		break;
		case USART_Tx_Rx:
		     UCSRB_Temp |= (1<<RXEN) | (1<<TXEN);
		break;
		default:
		break;
	}
	
	switch (UART0.CommunicationMode)
	{
		case USART_Async_NormalSpeed:
		     UBRR_Temp = (((float)F_CPU) / (16.0 * UART0.BaudRate)) - 0.5;
		break;
		case USART_Async_DoubleSpeed:
			 UCSRA_Temp |= (1<<U2X);
			 UBRR_Temp = (((float)F_CPU) / (8.0 * UART0.BaudRate)) - 0.5;
		break;
		case USART_Sync:
		     UCSRC_Temp |= (1<<UMSEL);
			 UBRR_Temp = (((float)F_CPU) / (2.0 * UART0.BaudRate)) - 0.5;
			 switch (UART0.ClockPolarity)
			 {
				 case USART_SampleOnFalling:
				 break;
				 case USART_SampleOnRising:
				  UCSRC_Temp |= (1<<UCPOL);
				 break;
			 }
		break;
		default:
		break;
	}
	switch(UART0.CommunicationTerminal)
	{
		case USART_SingleProcessor:
		break;
		case USART_MultiProcessor:
		     UCSRA_Temp |= (1<<MPCM);
		break;
		default:
		break;
	}
	
	switch(UART0.InterruptSource)
	{
		case USART_InterruptDisable:
		break;
		case USART_RxComp_Enable:
		     UCSRB_Temp |= (1<<RXCIE);
		break;
		case USART_TxComp_Enable:
		     UCSRB_Temp |= (1<<TXCIE);
		break;
		case USART_UDREmpty_Enable:
		     UCSRB_Temp |= (1<<UDRE);
		break;
		case USART_RxComp_TxComp_Enable:
			UCSRB_Temp |= (1<<TXCIE) | (1<<RXCIE);
		break;
		case USART_RxComp_UDREmpty_Enable:
		    UCSRB_Temp |= (1<<UDRE) | (1<<RXCIE);
		break;
		case USART_TxComp_UDREmpty_Enable:
		    UCSRB_Temp |= (1<<UDRE) | (1<<TXCIE);
		break;
		case USART_RxComp_TxComp_UDREmpty_Enable:
		    UCSRB_Temp |= (1<<UDRE) | (1<<TXCIE) | (1<<RXCIE);
		break;
		default:
		break;
	}
	
	switch(UART0.DataFrame)
	{
		case USART_5bitData:
		break;
		case USART_6bitData:
		     UCSRC_Temp |= (1<<UCSZ0);
		break;
		case USART_7bitData:
		     UCSRC_Temp |= (1<<UCSZ1);
		break;
		case USART_8bitData:
		     UCSRC_Temp |= (1<<UCSZ0) | (1<<UCSZ1);
		break;
		case USART_9bitData:
		     UCSRC_Temp |= (1<<UCSZ0) | (1<<UCSZ1);
			 UCSRB_Temp |= (1<<UCSZ2);
		break;
		default:
		break;
	}
	
	switch (UART0.ControlFrame)
	{
		case USART_ParityDisable_1Stop:
		break;
		case USART_ParityDisable_2Stop:
			 UCSRC_Temp |= (1<<USBS);
		break;
		case USART_ParityEven_1Stop:
		     UCSRC_Temp |= (1<<UPM1);
		break;
		case USART_ParityEven_2Stop:
		     UCSRC_Temp |= (1<<USBS) | (1<<UPM1);
		break;
		case USART_ParityOdd_1Stop:
		     UCSRC_Temp |= (1<<UPM0) | (1<<UPM1);
		break;
		case USART_ParityOdd_2Stop:
		     UCSRC_Temp |= (1<<USBS) | (1<<UPM0) | (1<<UPM1);
		break;
	}
	
	
	UCSRA = UCSRA_Temp;
	UCSRC = UCSRC_Temp;
	UCSRB = UCSRB_Temp;
	UBRRH = ((unsigned short)UBRR_Temp)>>8;
	UBRRL = ((unsigned char)UBRR_Temp) & 0xFF;
	
}


void USART_SendByte_Blocking(unsigned short Data)
{
	while((UCSRA & (1<<UDRE)) == 0){}
	
	if (UART0.DataFrame == USART_9bitData)
	{
		UCSRB = (UCSRB & 0xFE) | ((Data & (1<<8)) >> 8);
	}
	UDR = (unsigned char) Data;
}


void USART_SendByte_NonBlocking(unsigned short Data)
{
	if ((UCSRA & (1<<UDRE)) != 0)
	{
		if (UART0.DataFrame == USART_9bitData)
		{
			UCSRB = (UCSRB & 0xFE) | ((Data & (1<<8)) >> 8);
		}
		UDR = (unsigned char) Data;
	}
}


unsigned short USART_ReadByte_Blocking(void)
{
	unsigned short rxData = 0;
	
	while ( (UCSRA & (1<<RXC)) == 0){}  
		
	if (UCSRA & ((1<<FE) | (1<<DOR) | (1<<UPE)))
	{
		// Error : You Should Handle it as you Wish 
		if (UCSRA & (1<<FE))
		{
			UART0.ReadMsg_Error = USART_FrameError;
		}
		else if (UCSRA & (1<<DOR))
		{
			UART0.ReadMsg_Error = USART_OverRunError;
		}
		else
		{
			UART0.ReadMsg_Error = USART_ParityError;
		}
	}
	
	if (UART0.DataFrame == USART_9bitData)
	{
		rxData =  UCSRB & (1<<RXB8) << 7;
	}
	rxData |= UDR;
	return rxData;
}



unsigned short USART_ReadByte_NonBlocking(void)
{
	unsigned short rxData = 0;
	if( (UCSRA & (1<<RXC)) != 0)
	{
		
		if (UCSRA & ((1<<FE) | (1<<DOR) | (1<<UPE)))
		{
			// Error : You Should Handle it as you Wish
			if (UCSRA & (1<<FE))
			{
				UART0.ReadMsg_Error = USART_FrameError;
			}
			else if (UCSRA & (1<<DOR))
			{
				UART0.ReadMsg_Error = USART_OverRunError;
			}
			else
			{
				UART0.ReadMsg_Error = USART_ParityError;
			}
		}
	
		if (UART0.DataFrame == USART_9bitData)
		{
			rxData =  UCSRB & (1<<RXB8) << 7;
		}
		rxData |= UDR;
	}
	return rxData;
}


void USART_SendString_Blocking(char * StrAddress)
{
	
	while(*StrAddress != '\0')
	{
		USART_SendByte_Blocking(*StrAddress);
		StrAddress++;
	}
}