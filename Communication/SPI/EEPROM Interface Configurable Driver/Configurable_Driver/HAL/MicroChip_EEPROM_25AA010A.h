/*
 * MicroChip_EEPROM_25AA010A.h
 *
 * Created: 4/21/2020 1:10:54 AM
 *  Author: Hamdy
 */ 


#ifndef MICROCHIP_EEPROM_25AA010A_H_
#define MICROCHIP_EEPROM_25AA010A_H_


	#define CHIP_SELECT_PORT		PORTB
	#define CHIP_SELECT_DDR			DDRB
	#define CHIP_SELECT_PIN			PB0
	#define SPI_USED				SPI0
	
void EEPROM_Init(void);
void EEPROM_WriteByte(unsigned char Data , unsigned short address);
unsigned char EEPROM_ReadByte(unsigned short address);


#endif /* MICROCHIP_EEPROM_25AA010A_H_ */