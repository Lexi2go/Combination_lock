/*
 * main.h
 *
 * Created: 10.12.2022 21:13:46
 *  Author: lexi
 */ 


#ifndef MAIN_H_
#define MAIN_H_

// include's

#include <avr/io.h>
#include <avr/eeprom.h>
#include "Keys.h"
#include "LED.h"
#include "UART.h"
#include "Timer0.h"
#include "display_funktionen.h"


void Code_falsch();
void Code_richtig();

unsigned char ucWert = 0x1A;
unsigned int uiWert = 0x2345;



#endif /* MAIN_H_ */