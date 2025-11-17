/*
 * Codeschloss.c
 *
 * Created: 17.10.2022 12:20:43
 * Author : lexi
 */ 

#include "main.h"




unsigned char ucCode[4];					// Array für eingegebenen Code 
unsigned char ucInkrement;					// Wert des Codes 1,2,3,4,5,6,7,8,9,0
unsigned char ucStelle=0;					// Spalte des Codes 1-4

unsigned char ucMerkerAnf = 0;				// Merker für Anfangszustand, beim Einschalten ("alter Code: 1111")
unsigned char ucMerker1=0;					// Merker für Display-Ausgabe ("alter Code:")
unsigned char ucMerker2=0;					// Merker für Display-Ausgabe ("falscher Code") 
unsigned char ucMerker3=0;					// Merker für Abarbeitung der Reihenfolge 
unsigned char ucMerker4=0;					// Merker für  
unsigned char ucMerker5 = 0;				// Merker für 

unsigned char i;							// Zählvariable, um im Array die einzelnen Zahlen aufzurufen
unsigned char ucFehlervariable;				// Variable wird auf 1 gesetzt, wenn der eingegebene Code mit dem Code im EEPROM nicht übereinstimmt

unsigned char uckey;						// Variable für Tasterabfrage
unsigned char pucEPByte[4] EEMEM;			// Zeiger auf ein Feld im EEPROM
unsigned char ucBegin[4]="1111";			// char Feld deklarieren für EEPROM Speicher, plus 48
unsigned char ucFeld[4];					// Array für Abfrage EEPROM Feld

int main(void)
{
	
	Display_Clear();													// Display löschen
	eeprom_is_ready();													// EEPROM ist bereit 
	eeprom_update_block(ucBegin,pucEPByte,4);							// Vorbelegung des Codes im EEPROM mit "1111"
	ucMerkerAnf = 1;													// Merker für Anfangszustand 
	
    /* Replace with your application code */
    while (1) 
    {
		if(Timer0_get_100msState()== TIMER_TRIGGERED)
		{
			
			uckey = Keys_get_state();								// Variable für die Tasterzustände
			switch (uckey)
			{				
				case KEYS_NOT_PRESSED:								// Keine Taste gedrückt 
					
					Schriftweg();									// Ausblenden der Texte
					
					if(ucMerkerAnf == 1)							// Anfangszustand nach, 1. Einschalten
					{
						Display_SetCursor(0,0);
						Display_Print("alter Code: 1111",16);
					}
					
					if (ucMerker1==1)
					{								
						Display_SetCursor(0,0);
						Display_Print("alter Code:     ",16);				// Ausgabe "alter Code"
						Display_SetCursor(1,0);
						Display_Print("                ",16);
						if(ucMerker2 == 1)									// eingegebener Code ist falsch zum EEPROM Code
						{
							Code_falsch();									// rote LED blinkt
						}
						if (ucMerker5 == 1)									// nach speichern des "neuen Codes" (Drücken des 3.Tasters)
						{
							Code_in_EEPROM();								// Display Ausgabe ("neu")
						}
					}
					
					if(ucMerker3 == 1)												
					{
						Display_SetCursor(0,0);
						Display_Print("neuer Code:     ",16);
						if (ucMerker4 == 1)												// nach erfolgreichem Eingeben des alten Codes
						{
							Code_richtig();
						}
					}
					
					break;
				
				case S1_PRESSED:														// Hochzählen von Null bis 9
					alleMerker0();
					alleLEDs_Aus();
					Schriftweg();
					
					ucInkrement++;																
					if (ucInkrement==10)
					{
						ucInkrement=0;
					}
					Display_SetCursor(1,ucStelle);		//ucStelle 0 -4
					Display_Write(ucInkrement+48);
				
					ucCode[ucStelle] = ucInkrement+48;
					break;
				
				case S2_PRESSED:														// Horizontale Verschiebung des Cursors auf die nächste Spalte
					Schriftweg();
					alleLEDs_Aus();
					alleMerker0();
					
										
					// 1 Stelle nach rechts 
					ucInkrement = 0;
					ucStelle++;
					if(ucStelle == 4) ucStelle = 0;
				
				break;
				
				case S3_PRESSED:															// Vergleichen des Codes und Speichern 
					Schriftweg();
					eeprom_read_block(ucFeld, pucEPByte,4);
					ucFehlervariable = 0;
					
					
					switch (ucMerker3)
					{
					case 0:
						for(i=0;i<4;i++)
						{
							if(ucFeld[i] != ucCode[i])						// Vergleich von Code im EEPROM(ucFeld) zum eingegebenen Code (ucCode)
							{
								ucFehlervariable = 1;						// wenn Code falsch ist
							}
						}
						if(ucFehlervariable == 0)							// Vergleich von EEPROM mit Eingabe Code korrekt
						{
							UART_send_Data(" Code korrekt ");				// über UART wird Botschaft "Code korrekt" gesendet
							ucMerker4 = 1;									// ucMerker4 = 1, 							
							ucMerker3 = 1;									// ucMerker3 = 1, springt im nächsten Durchlauf in case 1
						}
						else if (ucFehlervariable == 1)						// Vergleich von EEPROM mit Eingabe Code falsch
						{
							UART_send_Data(" Code falsch ");
							ucMerker1 = 1;
							ucMerker2 = 1;
						}
						break;												// immer 1 eingerückt
				
					case 1:													// Einagbe korrekt
						cli();
						for(i=0;i<4;i++)
						{
							ucFeld[i] = ucCode[i];
						}
						eeprom_update_block(ucFeld, pucEPByte, 4);			// EEPROM mit eingegebenen Code updaten
						UART_send_Data(" neuer Code ");						// UART sendet "neuer Code"
						sei();
						
						ucMerker1 = 1;										// Setzen von ucMerker1
						ucMerker3 = 0;										// Zurücksetzen ucMerker3 = 0, springt im nächsten Durchlauf in case 0
						ucMerker5 = 1;										// Setzen von ucMerker5, Display gibt im nächsten Schleifendurchlauf "alter Code: neu" 
																			 
						break;
					}
				
				break;
				
			/*	case S4_PRESSED:														// ESC (Zurück Taste)
					ucMerker2=0;
					ucMerker1=1;														// ucMerker1 = 1 -> springt in 2. If-Schleife 
				
				break;*/
			}
		}
    }
}

// Definition der Funktionen 
//
//------------------------------------------------------------------------------------------------------------------------------------------
//Funktion: Init
//Beschreibung: Funktion ruft alle Funktionen zur Initialisierung auf
//Aufgerufene Funktionen: LED_Init, 
//						  Timer0_Init, 
//						  Display_Init, 
//						  UART_Init
//Übergebene Parameter	: keine
//Zurückgegebene Werte	: keine
//------------------------------------------------------------------------------------------------------------------------------------------
void Init()
{
	LED_Init();															// Initialisierung der LEDs rot und grün
	Timer0_Init();														// Initialisierung des Timer0
	Display_Init();														// Initialisierung des Display
	UART_Init();														// Initialisierung der UART-Schnittstelle
}
//
//------------------------------------------------------------------------------------------------------------------------------------------
//Funktion: alleMerker0
//Beschreibung: alle Merker werden auf 0 gesetzt
//Aufgerufene Funktionen: keine
//Übergebene Parameter	: keine
//Zurückgegebene Werte	: 0
//------------------------------------------------------------------------------------------------------------------------------------------
void alleMerker0 ()
{
	ucMerkerAnf = 0;
	ucMerker1 = 0;														
	ucMerker2 = 0;
	ucMerker4 = 0;
	ucMerker5 = 0;
}
//
//------------------------------------------------------------------------------------------------------------------------------------------
//Funktion: alleLEDs_Aus
//Beschreibung: Funktion schaltet rote und grüne LED aus
//Aufgerufene Funktionen: LED_ro_off,
//						  LED_gr_off
//Übergebene Parameter	: keine
//Zurückgegebene Werte	: keine
//------------------------------------------------------------------------------------------------------------------------------------------
void alleLEDs_Aus()
{
	LED_ro_off();
	LED_gr_off();
}
//
//------------------------------------------------------------------------------------------------------------------------------------------
//Funktion: Code_falsch()
//Beschreibung: Funktion triggert rote LED jede 90ms und gibt auf Display "Code falsch" aus
//Aufgerufene Funktionen: Display_SetCursor,
//						  Display_Print,
//						  LED_ro_toggle,
//						  Timer0_get_90ms_State
//Übergebene Parameter	: keine
//Zurückgegebene Werte	: keine
//------------------------------------------------------------------------------------------------------------------------------------------
void Code_falsch()
{
	Display_SetCursor(1,4);
	Display_Print("Code falsch!",12);								// Display gibt in Zeile 1 "Code falsch" aus
	if(Timer0_get_90msState()== TIMER_TRIGGERED)
	{
		LED_ro_toggle();											// Toggeln der LED alle 90ms
	}
}
//
//------------------------------------------------------------------------------------------------------------------------------------------
//Funktion: Code_richtig
//Beschreibung: Funktion schaltet grüne LED an
//Aufgerufene Funktionen: LED_gr_on
//Übergebene Parameter	: keine
//Zurückgegebene Werte	: keine
//------------------------------------------------------------------------------------------------------------------------------------------
void Code_richtig()
{
	LED_gr_on();									// LED grün an
}
//
//------------------------------------------------------------------------------------------------------------------------------------------
//Funktion: Code_in_EEPROM
//Beschreibung: Funktion gibt "neu" auf dem Display aus(Zeile 0, ab Spalte 12)
//Aufgerufene Funktionen: Display_SetCursor,
//						  Display_Print
//Übergebene Parameter	: keine
//Zurückgegebene Werte	: keine
//------------------------------------------------------------------------------------------------------------------------------------------
void Code_in_EEPROM()
{
	Display_SetCursor(0,12);
	Display_Print(" neu",4);						// wenn Code in EEPROM gespeichert wird, wird "neu" in Zeile 0 hinzugefügt 
}
//
//------------------------------------------------------------------------------------------------------------------------------------------
//Funktion: Schriftweg
//Beschreibung: Funktion blendet Text in Zeile 0 und Zeile 1 ab 4. Spalte aus
//Aufgerufene Funktionen: Display_SetCursor,
//						  Display_Print
//Übergebene Parameter	: keine
//Zurückgegebene Werte	: keine
//------------------------------------------------------------------------------------------------------------------------------------------
void Schriftweg()
{
	Display_SetCursor(0,0);
	Display_Print("                ", 16);			//Ausblenden Zeile 0
	Display_SetCursor(1,4);
	Display_Print("            ",12);				// Ausblenden Zeile 1, ab 4. Spalte
}

