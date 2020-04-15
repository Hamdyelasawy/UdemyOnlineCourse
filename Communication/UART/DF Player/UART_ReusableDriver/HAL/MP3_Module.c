/*
 * MP3_Module.c
 *
 * Created: 4/15/2020 3:28:00 AM
 *  Author: Hamdy
 */ 

#include "uc_Configuration.h"
#include "ATmega32A_UART.h"
#include <util/delay.h>

#define MP3_CMD_SIZE	10
#define MP3_STARTBYTE   0x7E
#define MP3_VERSION		0xFF
#define MP3_NOFEEDBACK		0x00
#define MP3_RQTFEEDBACK		0x01

#define MP3_ENDBYTE			0xEF
/* List of MP3 Commands */
#define MP3_CMD_PlAYBACK	0x03
#define MP3_CMD_SETVOLUME	0x06
#define MP3_CMD_RESET       0x0C
#define MP3_CMD_PLAY		0x0D
#define MP3_CMD_PAUSE		0x0E


unsigned char Mp3_DefaultCommand[MP3_CMD_SIZE] = {MP3_STARTBYTE,MP3_VERSION,6,0,MP3_NOFEEDBACK,0,0,0,0,MP3_ENDBYTE};


static void SendCommandFrame(void)
{
	unsigned char i =0;
	unsigned short CheckSum = 0;
	unsigned short Add_Frame = 0;
	for(i=0;i<10;i++)
	{
		
		if ( i > 0 && i < 7 )
		{
			Add_Frame += Mp3_DefaultCommand[i];
		}
		else if ( i == 7)
		{
		  	CheckSum = 0xFFFF  -  Add_Frame  + 1 ;
			
			Mp3_DefaultCommand[7] = CheckSum >> 8 ;
			Mp3_DefaultCommand[8] = (unsigned char) CheckSum;
		}
		USART_SendByte_Blocking(Mp3_DefaultCommand[i]);
	}
}

void Mp3_Init(void)
{
	/* Configure USART During RunTime */
	UART0.BaudRate = 9600;
	UART0.EnableMode = USART_Tx_Only;
	UART0.CommunicationMode = USART_Async_DoubleSpeed;
	UART0.CommunicationTerminal = USART_SingleProcessor;
	UART0.ControlFrame = USART_ParityDisable_1Stop;
	UART0.DataFrame = USART_8bitData;
	UART0.InterruptSource = USART_InterruptDisable;
	/* Init USART */
	USART_Init();
	/* MP3 Player ONLINE */
	Mp3_DefaultCommand[3] = MP3_CMD_RESET;
	SendCommandFrame();
	/* return To Default Value */
	Mp3_DefaultCommand[3] = 0;
	/* Delay 1 Sec to make Sure Mp3 Module is Online */
	_delay_ms(1000);
}

void Mp3_PlayTrack(unsigned short TrackNumber)
{
	Mp3_DefaultCommand[3] = MP3_CMD_PlAYBACK;
	Mp3_DefaultCommand[5] = TrackNumber >> 8;
	Mp3_DefaultCommand[6] = (unsigned char) TrackNumber;
	
	SendCommandFrame();
	
	Mp3_DefaultCommand[3] = 0;
	Mp3_DefaultCommand[5] = 0;
	Mp3_DefaultCommand[6] = 0;
}
void Mp3_SelectVolume(unsigned short VolumeLevel)
{
		Mp3_DefaultCommand[3] = MP3_CMD_SETVOLUME;
		Mp3_DefaultCommand[5] = VolumeLevel >> 8;
		Mp3_DefaultCommand[6] = (unsigned char) VolumeLevel;
		
		SendCommandFrame();
		
		Mp3_DefaultCommand[3] = 0;
		Mp3_DefaultCommand[5] = 0;
		Mp3_DefaultCommand[6] = 0;
}