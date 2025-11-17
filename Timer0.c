/*
 * Timer0.c
 *
 * Created: 13.11.2021 14:37:56
 *  Author: lexi
 */ 

//include's
#include "Timer0.h"

//global deklarierte Variablen
unsigned char ucCntSec = 0,ucCntSec90 = 0, ucCntSec1s = 0;	// zählt jede 10ms bis 1s erreicht wird, wird 1
unsigned char ucFlag10ms = 0;				// wird 1 wenn 1ms erreicht ist
unsigned char ucFlag90ms = 0;
unsigned char ucFlag100ms = 0;
unsigned char ucFlag1s = 0;					// wird 1 wenn 1s vergangen ist


// Definition der Funktionen

void Timer0_Init(void)
{
	TCCR0A |= 1 << WGM01; //CTC-Modus
	TCCR0B |= (1 << CS00) | (1 << CS02); //Prescaler = 1024
	OCR0A = 179; //10 ms @ 18,432 MHz
	TIMSK0 |= 1 << OCIE0A; //Interrupt wird freigegeben
	sei(); //alle Interrupts werden freigegeben
}

ISR(TIMER0_COMPA_vect) // alle 10 ms wird vom Timer ausgelöst
{
	ucFlag10ms = 1; //wenn gesetzt, sind 10 ms vergangen
	ucCntSec++;
	ucCntSec90++;
	ucCntSec1s++;
	if(ucCntSec90 == 9)
	{
		ucCntSec90 = 0;
		ucFlag90ms = 1;
	}
	if(ucCntSec == 10)
	{
		ucCntSec = 0;
		ucFlag100ms = 1;
	}
	if(ucCntSec1s == 100) // alle 1s blinkt/ leuchtet die LED
	{
		ucCntSec1s = 0;
		ucFlag1s = 1;
		
	}
}

unsigned char Timer0_get_10msState(void) // Funktion gibt zurück, ob 10ms verstrichen ist
{
	if(ucFlag10ms == 1)
	{
		ucFlag10ms = 0;
		return TIMER_TRIGGERED;
	}
	return TIMER_RUNNING;
}

unsigned char Timer0_get_1sState(void) // Funktion gibt zurück, ob 1s verstrichen ist
{
	if(ucFlag1s == 1)
	{
		ucFlag1s = 0;
		return TIMER_TRIGGERED;
	}
	return TIMER_RUNNING;
}
unsigned char Timer0_get_90msState(void)
{
	if(ucFlag90ms ==1)
	{
		ucFlag90ms = 0;
		return TIMER_TRIGGERED;
	}
	return TIMER_RUNNING;
}

unsigned char Timer0_get_100msState(void)
{
	if(ucFlag100ms ==1)
	{
		ucFlag100ms = 0;
		return TIMER_TRIGGERED;
	}
	return TIMER_RUNNING;
}
