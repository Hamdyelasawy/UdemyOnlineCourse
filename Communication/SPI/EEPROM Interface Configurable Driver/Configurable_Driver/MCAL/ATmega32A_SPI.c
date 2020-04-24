/*
 * ATmega32A_SPI.c
 *
 * Created: 4/19/2020 11:33:02 AM
 *  Author: Hamdy
 */ 

#include "uc_Configuration.h"
#include "ATmega32A_SPI.h"
#include <avr/io.h>
#include <avr/interrupt.h>


ISR(SPI_STC_vect)
{
	if (SPI0.SPI_TransfereInterruptCall != NULL_PTR)
	{
		SPI0.SPI_TransfereInterruptCall();
	}
}

	SPI_ConfigStruct SPI0 = {.SPI_TransfereInterruptCall = NULL_PTR};

	void SPI_Init(void)
	{
		SPCR = 0;
		SPSR = 0;
		switch(SPI0.EnableControl)
		{
			case SPI_Disable:
			break;
			case SPI_Enable:
				 switch(SPI0.Mode)
				 {
					 case SPI_MASTER:
					 SPCR |= (1<<MSTR);
					 DDRB |= (1<<5) | (1<<7) | (1<<4);
					 PORTB |= (1<<PB4) | (1<<5);
					 break;
					 case SPI_SLAVE:
					 DDRB |= (1<<6);
					 break;
				 }
				 switch(SPI0.DataOrder)
				 {
					 case SPI_LSB:
					 SPCR |= (1<<DORD);
					 break;
					 case SPI_MSB:
					 break;
				 }
				 switch(SPI0.InterruptState)
				 {
					 case SPI_InterruptDisable:
					 break;
					 case SPI_InterruptEnable:
					 SPCR |= (1<<SPIE);
					 break;
				 }
				 switch(SPI0.ClockMode)
				 {
					 case SPI_CLKMode0:
					 break;
					 case SPI_CLKMode1:
					 SPCR |= (1<<CPHA);
					 break;
					 case SPI_CLKMode2:
					 SPCR |= (1<<CPOL);
					 	/* 
						 * Note : if Used SPI Mode 3 or Mode 2 (CLK IDLE STATE = HIGH)
						 * in Some MicroControllers we must Explicitly State that The Default clk pin state is High 
						 * By Setting the pin to Output with a High Value 
						 */
					PORTB |= (1<<7);
					 break;
					 case SPI_CLKMode3:
					 SPCR |= (1<<CPHA) | (1<<CPOL);
					PORTB |= (1<<7);
					 break;
				 }
				 switch(SPI0.Freq)
				 {
					 case SPI_FCPU_BY_2:
					 SPSR |= (1<<SPI2X);
					 break;
					 case SPI_FCPU_BY_4:
					 break;
					 case SPI_FCPU_BY_8:
					 SPSR |= (1<<SPI2X);
					 SPCR |= (1<<SPR0);
					 break;
					 case SPI_FCPU_BY_16:
					 SPCR |= (1<<SPR0);
					 break;
					 case SPI_FCPU_BY_32:
					 SPSR |= (1<<SPI2X);
					 SPCR |= (1<<SPR1);
					 break;
					 case SPI_FCPU_BY_64:
					 SPCR |= (1<<SPR1);
					 break;
					 case SPI_FCPU_BY_128:
					 SPCR |= (1<<SPR1) | (1<<SPR0);
					 break;
				 }
			/* Enable SPI Communication */
			SPCR |= (1<<SPE);
			break;
		}
	}
	
	
	unsigned char SPI_ReadWrite_Blocking(unsigned char Data)
	{
		/* Transmit Data Using SPI Bus */
		SPDR = Data;
		
		/* Check if a Write Collision Error Occurred */
		if ( (SPSR & (1<<WCOL) ) != 0)
		{
			SPI0.Error = SPI_WriteCollison;
		}
		
		/* Block till The End of Transmit ion */
		while ( (SPSR & (1<<SPIF)) == 0 ) {}
		
		/* return Rx Data From SPI */
		return SPDR;
	}
	
	
	unsigned char SPI_ReadWrite_NonBlocking(unsigned char Data)
	{
				/* Transmit Data Using SPI Bus */
				SPDR = Data;
				/* Check if a Write Collision Error Occurred */
				if ( (SPSR & (1<<WCOL))  != 0)
				{
					SPI0.Error = SPI_WriteCollison;
				}
				/* return Rx Data From SPI */
				return SPDR;	
	}
