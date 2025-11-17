/*
 * Display.c
 *
 * Created: 12.12.2022 20:02:05
 *  Author: lexi
 */ 

/*
#include "Keys.h"
#include "UART.h"

unsigned char ucDisp_Merker;
unsigned char ucInkrement;
unsigned char ucZahl;
unsigned char ucCode[i] = 4;
unsigned char ucZahl[b] = 10;
unsigned char ucMenue;

void Hoch_S1(void)
{
	if(S1_PRESSED)
	{
		
	switch (uckey)
	{
		case KEYS_NOT_PRESSED:
		if(ucDisp_Merker ==0)
		{
			
			Display_SetCursor(1,1);
			Display_Print("             ",15);
			ucDisp_Merker = 1;
		}break;
		
		case S1_PRESSED:
		if(ucInkrement ==10)
		{
			ucInkrement ==0;
		}
		//if(ucInkrement ==0
		if(ucInkrement ==0)
		{	
			Display_SetCursor();
			Display_Print("1",1);				// Zahl 1 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[ucInkrement] = ucInkrement + 48;							// Merkwert 1
		}
		
		if(ucInkrement ==1)
		{
			Display_SetCursor();
			Display_Print("2",1);				// Zahl 2 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[ucInkrement] = ucInkrement + 48;									// Merkwert 1
		}
		
		if(ucInkrement ==2)
		{
			Display_SetCursor();
			Display_Print("3",1);				// Zahl 2 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[ucInkrement] = ucInkrement + 48;									// Merkwert 1
		}
		
		if(ucInkrement ==3)
		{
			Display_SetCursor();
			Display_Print("1",1);				// Zahl 1 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[ucInkrement] = ucInkrement + 48;									// Merkwert 1
		}
		
		if(ucInkrement ==4)
		{
			Display_SetCursor();
			Display_Print("2",1);				// Zahl 2 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[ucInkrement] = ucInkrement + 48;									// Merkwert 1
		}
		
		if(ucInkrement ==5)
		{
			Display_SetCursor();
			Display_Print("3",1);				// Zahl 2 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[ucInkrement] = ucInkrement + 48;									// Merkwert 1
		}
		
		if(ucInkrement ==6)
		{
			Display_SetCursor();
			Display_Print("2",1);				// Zahl 2 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[ucInkrement] = ucInkrement + 48;									// Merkwert 1
		}
		
		if(ucInkrement ==7)
		{
			Display_SetCursor();
			Display_Print("3",1);				// Zahl 2 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[ucInkrement] = ucInkrement + 48;									// Merkwert 1
		}
		
		if(ucInkrement ==8)
		{
			Display_SetCursor();
			Display_Print("1",1);				// Zahl 1 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[ucInkrement] = ucInkrement + 48;									// Merkwert 1
		}
		
		Inkrement++;
		break;
		
		case S2_PRESSED:
		
		ucMenue;
		
		ucInkrement = 0;
		
	}
	
	
}*/