/*
 * MicroChip_EEPROM_25AA010A.c
 *
 * Created: 4/21/2020 1:10:44 AM
 *  Author: Hamdy
 */ 

#include "ATmega32A_SPI.h"
#include "MicroChip_EEPROM_25AA010A.h"
#include "uc_Configuration.h"
#include <util/delay.h>
#include <avr/io.h>

#define WREN_OPCODE		(0x06)
#define WRTIE_OPCODE	(0x02)
#define READ_OPCODE		(0x03)

#define DUMMY_DATA		(0x00)

static void SPI_CS_Control(unsigned char State)
{
	switch(State)
	{
		case 0:
		CHIP_SELECT_PORT &= ~(1<<CHIP_SELECT_PIN);
		break;
		case 1:
		CHIP_SELECT_PORT |= (1<<CHIP_SELECT_PIN);
		break;
		default:
		break;
	}	
}

void EEPROM_Init(void)
{
	SPI_USED.EnableControl = SPI_Enable;
	SPI_USED.Mode = SPI_MASTER;
	/* 
	 * Note : if Used SPI Mode 3 or Mode 2 (CLK IDLE STATE = HIGH)
	 * in Some MicroControllers we must Explicitly State that The Default clk pin state is High 
	 * By Setting the pin to Output with a High Value 
	 */
	SPI_USED.ClockMode = SPI_CLKMode3;
	SPI_USED.DataOrder = SPI_MSB;
	SPI_USED.Freq = SPI_FCPU_BY_8;
	SPI_USED.InterruptState = SPI_InterruptDisable;
	/* Config CS Pin  to be in output mode to use it as CS */
	CHIP_SELECT_DDR |= (1<<CHIP_SELECT_PIN);
	CHIP_SELECT_PORT |= (1<<CHIP_SELECT_PIN);
	SPI_Init();
}

void EEPROM_WriteByte(unsigned char Data , unsigned short address)
{
	/* Select EEPROM By Applying 0 to CS */
	SPI_CS_Control(0);
	/* Write Latch Enable */
	SPI_ReadWrite_Blocking(WREN_OPCODE);
	/* Apply High to CS to Make Write Latch Enable Instruction Executed*/
	SPI_CS_Control(1);
	_delay_ms(50);
	/* Select EEPROM By Applying 0 to CS */
	SPI_CS_Control(0);
	SPI_ReadWrite_Blocking(WRTIE_OPCODE);
	SPI_ReadWrite_Blocking(address);
	SPI_ReadWrite_Blocking(Data);
	SPI_CS_Control(1);
}

unsigned char EEPROM_ReadByte(unsigned short address)
{
	
	unsigned char ReadData = 0;
	SPI_CS_Control(0);
	SPI_ReadWrite_Blocking(READ_OPCODE);
	SPI_ReadWrite_Blocking(address);
	ReadData = SPI_ReadWrite_Blocking(DUMMY_DATA);
	SPI_CS_Control(1);
	return ReadData;
}