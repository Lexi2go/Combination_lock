/*
 * LED.c
 *
 * Created: 13.11.2022 11:01:45
 *  Author: lexi
 */ 
// include's

#include "LED.h"

// Deklaration der globalen Variablen im Modul LED
//-----------------------------------------------------
// Defintion der Funktionen 
// Beschreibung: 
// Rückgabewert:
// Aufrufparameter:
// Aufgerufene Funktionen:
// Version:1.0
// Status: freigegeben
// ------------------------------------------------------

// Defintion der Funktionen 

void LED_Init(void)
{
	DDRB |= 1 << PB2; // PIN PB8 wird auf Ausgang gesetzt
	DDRD |= 1 << PD6; // PIN PD6 wird auf Ausgang gesetzt
}

void LED_gr_on(void)
{
	PORTD |= 1 << PD6; // grüne LED wird eingeschaltet
}

void LED_gr_off(void)
{
	
	PORTD &= ~(1 << PD6); // grüne LED wird ausgeschaltet
}

void LED_ro_on(void)
{
	PORTB |= 1 << PB2; // rote LED wird eingeschaltet
}

void LED_ro_off(void)
{
	PORTB &= ~(1 << PB2); // rote LED wird ausgeschaltet
}

void LED_ro_toggle(void)
{
	PORTB ^= (1 << PB2);
}