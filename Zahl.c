/*
 * Zahl.c
 *
 * Created: 15.12.2022 10:07:55
 *  Author: Lexi
 */ 

#include <avr/io.h>

void Hoch_S1(void)
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
		if(ucInkrement ==9)
		{
			ucInkrement ==0;
		}
		if(ucInkrement ==0)
		{
			Display_SetCursor();
			Display_Print("1",1);				// Zahl 1 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[0] = 1;							// Merkwert 1
		}
		
		if(ucInkrement ==1)
		{
			Display_SetCursor();
			Display_Print("2",1);				// Zahl 2 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[0] = 2;							// Merkwert 1
		}
		
		if(ucInkrement ==2)
		{
			Display_SetCursor();
			Display_Print("3",1);				// Zahl 2 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[0] = 3;							// Merkwert 1
		}
		
		if(ucInkrement ==3)
		{
			Display_SetCursor();
			Display_Print("1",1);				// Zahl 1 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[0] = 4;							// Merkwert 1
		}
		
		if(ucInkrement ==4)
		{
			Display_SetCursor();
			Display_Print("2",1);				// Zahl 2 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[0] = 5;							// Merkwert 1
		}
		
		if(ucInkrement ==5)
		{
			Display_SetCursor();
			Display_Print("3",1);				// Zahl 2 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[0] = 6;							// Merkwert 1
		}
		
		if(ucInkrement ==6)
		{
			Display_SetCursor();
			Display_Print("2",1);				// Zahl 2 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[0] = 7;							// Merkwert 1
		}
		
		if(ucInkrement ==7)
		{
			Display_SetCursor();
			Display_Print("3",1);				// Zahl 2 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[0] = 8;							// Merkwert 1
		}
		
		if(ucInkrement ==8)
		{
			Display_SetCursor();
			Display_Print("1",1);				// Zahl 1 Ausgabe
			Display_SetCursor(1,5);
			Display_Print("           ",11);
			ucCode[0] = 9;							// Merkwert 1
		}
		
		Inkrement++;
		break;
		
		case S2_PRESSED:
		
		ucMenue;
		
		Inkrement = 0;
		
	}
	
	
}