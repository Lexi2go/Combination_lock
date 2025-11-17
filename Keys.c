/*
 * Keys.c
 *
 * Created: 13.11.2022 15:26:50
 *  Author: Lexi
 */ 

//include's
#include "Keys.h"
unsigned char ucS1_Old, ucS1_New = 0xFF, ucS2_Old, ucS2_New = 0xFF; 
unsigned char ucS3_Old, ucS3_New = 0xFF, ucS4_Old, ucS4_New = 0xFF;

void Keys_Init(void)
{
	DDRD &= ~((1<<PD5) | (1<<PD4) | (1<<PD3) | (1<<PD2)); // PIN 2,3,4 und 5 vom Port D sind Eingänge
}


unsigned char Keys_get_state(void) // Entprellen und Abfragen der Tasten
{
	ucS1_Old = ucS1_New; // alter Zustand wird neuem Zustans zugewiesen
	ucS1_New = PIND & (1 << PD5); // Taste S1
	ucS2_Old = ucS2_New; // alter Zustand wird neuem Zustans zugewiesen
	ucS2_New = PIND & (1 << PD4); // Taste S2
	ucS3_Old = ucS3_New; // alter Zustand wird neuem Zustans zugewiesen
	ucS3_New = PIND & (1 << PD3); // Taste S3
	ucS4_Old = ucS4_New; // alter Zustand wird neuem Zustans zugewiesen
	ucS4_New = PIND & (1 << PD2); // Taste S4
	
	
	if(!ucS1_New && ucS1_Old) return S1_PRESSED;
	else if(!ucS2_New && ucS2_Old) return S2_PRESSED;
	else if(!ucS3_New && ucS3_Old) return S3_PRESSED;
	else if(!ucS4_New && ucS4_Old) return S4_PRESSED;
	return KEYS_NOT_PRESSED;
}
