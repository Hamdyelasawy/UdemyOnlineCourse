/*
 * ATmega32A_UART.h
 *
 * Created: 4/10/2020 5:30:19 AM
 *  Author: Hamdy
 */ 


/* 
	preProcessor Configurable Driver 
	Linking Configurable Driver 
	PostBuild Configurable Driver "During RunTime"
*/

#ifndef ATMEGA32A_UART_H_
#define ATMEGA32A_UART_H_

	typedef enum
	{
		USART_SingleProcessor,
		USART_MultiProcessor,
	}USART_CommunicationTerminal;

	typedef enum
	{
		USART_Async_NormalSpeed,
		USART_Async_DoubleSpeed,
		USART_Sync,
	}USART_CommunicationMode;

	typedef enum
	{
		USART_Disable,
		USART_Tx_Only,
		USART_Rx_Only,
		USART_Tx_Rx,
	}USART_EnableMode;
	
	typedef enum
	{
		USART_InterruptDisable,
		USART_RxComp_Enable,
		USART_TxComp_Enable,
		USART_UDREmpty_Enable,
		USART_RxComp_TxComp_Enable,
		USART_RxComp_UDREmpty_Enable,
		USART_TxComp_UDREmpty_Enable,
		USART_RxComp_TxComp_UDREmpty_Enable,
	}USART_InterruptSource;
	
	typedef enum
	{
		USART_5bitData,
		USART_6bitData,
		USART_7bitData,
		USART_8bitData,
		USART_9bitData,
	}USART_DataFrame;
	
	typedef enum
	{
		USART_ParityDisable_1Stop,
		USART_ParityDisable_2Stop,
		USART_ParityEven_1Stop,
		USART_ParityEven_2Stop,
		USART_ParityOdd_1Stop,
		USART_ParityOdd_2Stop,
	}USART_ControlFrame;
	
	typedef enum
	{
		USART_SampleOnFalling,
		USART_SampleOnRising,
	}USART_ClockPolarity;
	
	typedef enum 
	{
		USART_NoError,
		USART_ParityError,
		USART_OverRunError,
		USART_FrameError,
	}USART_ErrorType;
	
	typedef struct  
	{
		/* Please refer to Upper Enum For Details on What to Select */
		USART_EnableMode					EnableMode;
		USART_CommunicationMode				CommunicationMode;
		USART_CommunicationTerminal			CommunicationTerminal;
		USART_InterruptSource				InterruptSource;
		USART_DataFrame						DataFrame;
		USART_ControlFrame					ControlFrame;
		/* Assign This Option Only When Working in Sync Mode */
		USART_ClockPolarity					ClockPolarity;
		unsigned long						BaudRate;
		/* Read This Flag Before Using Data Returned by Read Function , Clear Flag Your Self */
		USART_ErrorType						ReadMsg_Error;
		/* Call Back Members */
		/* Make The Pointer points to a Function you wish to be Called by RXC Interrupt */
		void (*RXC_InterruptCall) (void);
		/* Make The Pointer points to a Function you wish to be Called by TXC Interrupt */
		void (*TXC_InterruptCall) (void);
		/* Make The Pointer points to a Function you wish to be Called by UDRE Interrupt */
		void (*UDRE_InterruptCall) (void);
	}USART_ConfigurationStruct;
	
	
	extern USART_ConfigurationStruct   UART0;
	
	void USART_Init(void);
    void USART_SendByte_Blocking(unsigned short Data);
	void USART_SendByte_NonBlocking(unsigned short Data);
	unsigned short USART_ReadByte_Blocking(void);
	unsigned short USART_ReadByte_NonBlocking(void);
	
#endif /* ATMEGA32A_UART_H_ */