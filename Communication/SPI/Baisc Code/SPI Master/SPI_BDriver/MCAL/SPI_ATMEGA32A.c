/*
 * SPI_ATMEGA32A.c
 *
 * Created: 4/18/2020 6:30:44 AM
 *  Author: Hamdy
 */ 

#include <avr/io.h>
#include "SPI_ATMEGA32A.h"
#include "uc_Configuration.h"

/* 
 * Master 
 * Freq = 250KHZ
 * SPI Mode 0
 * MSB First
 */

#define MOSI	5
#define SS		4
#define SCK		7
void SPI_Init(void)
{
	SPCR = (1<<SPE) | (1<<MSTR) ;
	/* In Master Mode User is Required to Set Direction of MOSI,SS and SCK pins */
	DDRB |= (1<< MOSI) | (1<<SS) | (1<<SCK);
}


void SPI_SendByte(unsigned char Data)
{
	/* Assign Data to SPDR To Start Sending Process */
	 SPDR = Data;
	/* Wait Till The Process End < Tx & Rx > */
	while ( (SPSR  & (1<<SPIF) ) == 0) {}
	
}

unsigned char SPI_ReadByte(void)
{
	unsigned char Dummy = 0x00;
	SPDR = Dummy;
	/* Wait Till The Process End < Tx & Rx > */
	while ( (SPSR  & (1<<SPIF) )  == 0) {}
	return SPDR;
}

unsigned char SPI_ExchangeData(unsigned char Data)
{
		SPDR = Data;
		/* Wait Till The Process End < Tx & Rx > */
		while ( (SPSR  & (1<<SPIF) )  == 0) {}
		return SPDR;
}
