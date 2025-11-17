/*
 * UART.h
 *
 * Created: 22.11.2021 14:41:17
 *  Author: Professor
 */ 


#ifndef UART_H_
#define UART_H_
//include's
#include <avr/io.h>
#include <avr/interrupt.h>

//define's
#define UART_RECEIVED		1
#define UART_NOT_RECEIVED	0

//Deklaration der Funktionen
void UART_Init(void);
unsigned char UART_get_State(void);
unsigned char UART_get_Value(void);
void UART_send_Byte(unsigned char ucbyte2send);
void UART_send_Data (char*data);




#endif /* UART_H_ */