/*
 * LED.h
 *
 * Created: 13.11.2022 10:48:12
 *  Author: lexi
 */ 


#ifndef LED_H_
#define LED_H_

// include's
#include <avr/io.h>

// define's


// Deklaration der Funktion
void LED_Init(void);
void LED_gr_on(void);
void LED_gr_off(void);
void LED_ro_on(void);
void LED_ro_off(void);
void LED_ro_toggle(void);



#endif /* LED_H_ */