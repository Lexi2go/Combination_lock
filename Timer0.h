/*
 * Timer0.h
 *
 * Created: 13.11.2022 10:43:35
 *  Author: Lexi
 */ 


#ifndef TIMER0_H_
#define TIMER0_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMER_TRIGGERED 1
#define TIMER_RUNNING 0

// Deklaration der Funktionen 

void Timer0_Init(void);
unsigned char Timer0_get_10msState(void);
unsigned char Timer0_get_100msState(void);
unsigned char Timer0_get_1sState(void);
unsigned char Timer0_get_90msState(void);




#endif /* TIMER0_H_ */