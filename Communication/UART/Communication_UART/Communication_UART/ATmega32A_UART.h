/*
 * ATmega32A_UART.h
 *
 * Created: 4/9/2020 8:05:57 AM
 *  Author: Hamdy
 */ 


#ifndef ATMEGA32A_UART_H_
#define ATMEGA32A_UART_H_

#define UART_BAUDRATE  (9600)


/* This Func Starts UART */
void UART_Init(void);
/* This Function Sends Byte Through UART */
void UART_SendByte(unsigned char Byte);
/* This Function Receive Byte Through UART*/
unsigned char UART_ReceiveByte(void);

#endif /* ATMEGA32A_UART_H_ */