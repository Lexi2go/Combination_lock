
/*
 * Keys.h
 *
 * Created: 13.11.2022 15:21:05
 *  Author: Lexi
 */ 


#ifndef KEYS_H_
#define KEYS_H_

#include <avr/io.h>

#define S1_PRESSED 1
#define S2_PRESSED 2
#define S3_PRESSED 3
#define S4_PRESSED 4
#define KEYS_NOT_PRESSED 0

void Keys_Init (void);
unsigned char Keys_get_state (void);




#endif /* KEYS_H_ */