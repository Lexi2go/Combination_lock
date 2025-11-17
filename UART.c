/*
 * UART.c
 *
 * Created: 14.11.2022 14:44:38
 *  Author: Lexi
 */ 

//include's
#include "UART.h"

//Deklaration der Variablen
unsigned char ucRX_Flag = 0;
unsigned char ucRX_Buffer = 0;
unsigned char ucSendIndex;
char *dataBuffer;


void UART_Init(void)
{
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);		// Sender und Empfänger eingeschalten
	UCSR0B |= (1 << RXCIE0) | (1 << TXCIE0) ;	// Interrupt zum empfangen und senden 
	UCSR0C |= (1 << UCSZ00) | (1 << UCSR0B);	// 2 Stoppbits
	UBRR0 = 119;
	sei();
}

unsigned char UART_get_State(void)
{
	if(ucRX_Flag)
	{
		ucRX_Flag = UART_NOT_RECEIVED;				// Reset Flag
		return UART_RECEIVED;
	}
	return UART_NOT_RECEIVED;
}

unsigned char UART_get_Value(void)
{
	return ucRX_Buffer;
}


void UART_send_Byte(unsigned char ucbyte2send)
{
	while(!(UCSR0A & (1 << UDRE0))); // warten bis das Senderegister frei ist
	UDR0 = ucbyte2send;
}

void UART_send_Data (char*data)
{
	dataBuffer = data;
	ucSendIndex =0;
	UDR0 = data[ucSendIndex++];
}

ISR(USART_RX_vect)
{
	ucRX_Buffer = UDR0;				// neues Datenwort empfangen
	ucRX_Flag = UART_RECEIVED;
}

ISR(USART_TX_vect)
{
	if(dataBuffer[ucSendIndex]) 
	UDR0 = dataBuffer[ucSendIndex++];
}