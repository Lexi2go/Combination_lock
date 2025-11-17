#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#define INT_PRESCALE 64L
#define CKDIV_VALUE   8L
#define PRESCALER	((1 << CS01) | (1 << CS00))

#define INT_CNTDWN_INIT (int) ((0.2L * F_CPU) / (CKDIV_VALUE * 256L * INT_PRESCALE))	/* Interrupt-Teiler-Countdown */

#define DOOR_TIMER_INIT 9	/* Vorgabewert für Türaktivierdauer (Wert im EEPROM) */
#define LED_CNTDWN_INIT 2	/* Countdown zur Herunterteilung des LED-Blinkens */
#define SCAN_TIMER_INIT 10	/* Vorgabewert für Timeout bei der Codeeingabe */
#define STARTUP_CNTDWN 6	/* Häufigkeit des LED-Blinkens beim Startup */
#define MAX_CODELENGTH 12	/* Maximale Länge des programmierbaren Codes */	

/* Falls auf Zielplattform compiliert werden soll */
#define TARGET_TINY2313

/* Falls Keypad immer genutzt werden soll, Wert auf 1 ändern */
#define FORCE_USE_KEYPAD 0

/* Falls LEDs und Türöffner low-aktiv angeschlossen sind, Wert auf 1 ändern */
#define LOW_ACTIVE 0

#ifdef TARGET_TINY2313

  /* Ports auf Zielplattform */

  /* Keybank 0 */
  #define KEYBANK0_PORT PORTB
  #define KEYBANK0_DDR  DDRB
  #define KEYBANK0_PIN  PINB
  #define KEYBANK0_MASK 0b00000000

  /* Keybank 1 */
  #define KEYBANK1_PORT PORTD
  #define KEYBANK1_DDR  DDRD
  #define KEYBANK1_PIN  PIND
  #define KEYBANK1_MASK 0b10111111

  /* Portpin für Programmiertaster auf Keybank 1 */
  #define PROG_KEY_PB PD6

  /* Portpin mit Steckbrücke zur Aktivierung des Keypads auf Keybank 0 */
  #define KEYPAD_ENABLE_PB PB7

  /* Tasten für Programmierung auf Keybank 0 */
  #define PROG_DOOR_TEST 	PORTB0
  #define PROG_DOOR_PLUS	PORTB1
  #define PROG_DOOR_MINUS	PORTB2

  /* Port und Portpins der LEDs; alle LEDs an einem Port */
  #define LED_PORT	  PORTD
  #define LED_DDR 	  DDRD
  #define LED_CODE_PB PORTD1
  #define LED_PROG_PB PORTD2
  #define LED_DOOR_PB PORTD3

  /* Türöffner */
  #define DOOR_PORT	PORTD
  #define DOOR_DDR	DDRD
  #define DOOR_PB	PORTD0

#else

  /* Ports auf Entwicklungsboard MyAVR */

  /* Keybank 0 */
  #define KEYBANK0_PORT PORTC
  #define KEYBANK0_DDR  DDRC
  #define KEYBANK0_PIN  PINC
  #define KEYBANK0_MASK 0b00000000

  /* Keybank 1 */
  #define KEYBANK1_PORT PORTD
  #define KEYBANK1_DDR  DDRD
  #define KEYBANK1_PIN  PIND
  #define KEYBANK1_MASK 0b00000000

  /* Portpin für Programmiertaster auf Keybank 1 */
  #define PROG_KEY_PB PORTD2

  /* Portpin mit Steckbrücke zur Aktivierung des Keypads auf Keybank 0 */
  #define KEYPAD_ENABLE_PB PORTC5

  /* Tasten für Programmierung auf Keybank 0 */
  #define PROG_DOOR_TEST 	PORTC1
  #define PROG_DOOR_PLUS	PORTC2
  #define PROG_DOOR_MINUS	PORTC3

  /* Port und Portpins der LEDs; alle LEDs an einem Port */
  #define LED_PORT	  PORTB
  #define LED_DDR 	  DDRB
  #define LED_CODE_PB PORTB0
  #define LED_PROG_PB PORTB1
  #define LED_DOOR_PB PORTB2

  /* Türöffner */
  #define DOOR_PORT	PORTB
  #define DOOR_DDR	DDRB
  #define DOOR_PB	PORTB3

#endif

#if (LOW_ACTIVE == 0)

  /* Makro für Einschalten einer LED */
  #define SET_LED_ON(LED) LED_PORT |= (LED);

  /* Makro für Ausschalten einer LED */
  #define SET_LED_OFF(LED) LED_PORT &= ~(LED);

  /* Makro für Einschalten des Türöffners */
  #define SET_DOOR_ON(DOOR)  DOOR_PORT |= (DOOR);

  /* Makro für Ausschalten des Türöffners */
  #define SET_DOOR_OFF(DOOR) DOOR_PORT &= ~(DOOR);

#else

  /* Makros für low-aktive Anschlüsse */
  #define SET_LED_ON(LED) LED_PORT &= ~(LED);
  #define SET_LED_OFF(LED)  LED_PORT |=  (LED);
  #define SET_DOOR_ON(DOOR)  DOOR_PORT &= ~(DOOR);
  #define SET_DOOR_OFF(DOOR) DOOR_PORT |= (DOOR);

#endif

/* Programmiertaste wurde losgelassen */
#define PROGKEYWASRELEASED (keyreleased1 & (1 << PROG_KEY_PB)) > 0

/* definierte Status der Statusmaschine */
enum {
	STATE_STARTUP = 0,
	STATE_READ_CODE = 1,
	STATE_DOOR_ENABLED = 2,
	STATE_PROG_CODE = 3,
	STATE_PROG_DOOR = 4
};

typedef uint8_t (*pFunc)(void);

volatile uint8_t status;		/* Status des Codeschlosses */

volatile uint8_t scancntdwn; 	/* Countdown-Zähler für Codeeingabe */
volatile uint8_t startcntdwn;	/* Countdown-Zähler für Startup */
volatile uint8_t doorcntdwn;	/* Countdown-Zähler für Türöffner */
volatile uint8_t ledcntdwn;     /* Countdown-Zähler für LED-Blinken */
volatile uint8_t int_cntdwn; 	/* Countdown-Zähler zur Herunterteilung im Interrupt */

uint8_t init_doorcntdwn;		/* Startwert für Türöffner-Countdown */

uint8_t code[MAX_CODELENGTH*2+2]; 	/* Code des Codeschlosses */
uint8_t *codeptr;

/* Countdown für Dauer der Türaktivierung */
uint8_t eeprom_initdoorcntdwn EEMEM = { DOOR_TIMER_INIT };

/* Tasten des Codeschlosses im EPROM */
uint8_t eeprom_code[MAX_CODELENGTH*2+2] EEMEM = { 0x00 };	

uint8_t keytemp0;					/* Tastenstatus Bank 0 */
uint8_t keytemp1;					/* Tastenstatus Bank 1 */
uint8_t keystate0;					/* entprellter Tastenstatus Bank 0 */
uint8_t keystate1;					/* entprellter Tastenstatus Bank 1 */
uint8_t keyvertcnt0a, keyvertcnt0b;	/* vertical counter Bank 0 */
uint8_t keyvertcnt1a, keyvertcnt1b; /* vertical counter Bank 1 */
volatile uint8_t keypressed0;
volatile uint8_t keyreleased0;
volatile uint8_t keypressed1;
volatile uint8_t keyreleased1;

uint8_t keychanged;

uint8_t usematrixkeypad;			/* Wert größer Null (True): Matrixtastatur benutzen */

/* Timer-Interrupt */
ISR(TIMER0_OVF_vect) {

	if (usematrixkeypad) {
		/* Matrixtastatur spaltenweise abfragen */
		KEYBANK0_PORT = 0b11101111;
		keytemp0 	  = KEYBANK0_PIN & 0b00001111;
		KEYBANK0_PORT = 0b11011111;
		keytemp0 	 |= ((KEYBANK0_PIN & 0b00001111) << 4);
		KEYBANK0_PORT = 0b10111111;
		keytemp1	  = KEYBANK0_PIN & 0b00001111;

		/* Programmierkontakt hinzuverknüpfen */
		keytemp1	 ^= KEYBANK1_PIN & (1 << PROG_KEY_PB);
	}
	else {
		keytemp0  = KEYBANK0_PIN | KEYBANK0_MASK;			/* nicht verwendete Tasten ausmaskieren durch High-setzen */
		keytemp1  = KEYBANK1_PIN | KEYBANK1_MASK;
	};
	
	/* Keybank 0 */
	keychanged    = keytemp0 ^ keystate0;		   			    /* geänderte Tasten */
	keyvertcnt0b  = (keyvertcnt0b ^ keyvertcnt0a) & keychanged;	/* vertical counter high - increment */
	keyvertcnt0a  = ~keyvertcnt0a & keychanged;	 				/* vertical counter low  - increment */
	keypressed0	 |= keyvertcnt0b & keystate0;
	keyreleased0 |= keyvertcnt0b & ~keystate0;
	keystate0 	 ^= keyvertcnt0b;
	
	/* keybank 1 */
	keychanged	  = keytemp1 ^ keystate1;
	keyvertcnt1b  = (keyvertcnt1b ^ keyvertcnt1a) & keychanged;
	keyvertcnt1a  = ~keyvertcnt1a & keychanged;
	keypressed1	 |= keyvertcnt1b & keystate1;
	keyreleased1 |= keyvertcnt1b & ~keystate1;
	keystate1	 ^= keyvertcnt1b;

	/* Countdown-Zähler zur Herunterteilung für langsame Vorgänge */

	if (int_cntdwn-- == 0) {

		/* hier stehen heruntergeteilte Vorgänge */
				
		/* Startup */
		if ((status == STATE_STARTUP) && (startcntdwn > 0)) {
			/* LEDs blinken mit weiter heruntergeteiltem Takt, Countdown hierfür herunterzählen */
			if (--ledcntdwn == 0) {	
				LED_PORT ^= (1 << LED_CODE_PB) | (1 << LED_PROG_PB) | (1 << LED_DOOR_PB);
				ledcntdwn = LED_CNTDWN_INIT;	/* Countdown wieder laden */
				startcntdwn--;					
			};				
		};

		/* Programmierung der Türaktivierdauer: Programmier-LED blinken lassen */
		if (status == STATE_PROG_DOOR) {
			if (--ledcntdwn == 0) {	
				LED_PORT ^= (1 << LED_PROG_PB);
				ledcntdwn = LED_CNTDWN_INIT;	/* Countdown wieder laden */
			};
		};
		
		/* Countdown-Zähler für Codeeingabe herunterzählen */
		if (scancntdwn > 0) scancntdwn--;
		
		/* Wenn Countdown für Türöffner auf Null läuft, Türöffner deaktivieren */
		if (doorcntdwn > 0) {
			if (--doorcntdwn == 0) {
				SET_DOOR_OFF(1 << DOOR_PB)
				SET_LED_OFF((1 << LED_CODE_PB) | (1 << LED_DOOR_PB))
			};
		};
		
		/* Countdown-Zähler für Herunterteilung neu laden */
		int_cntdwn = INT_CNTDWN_INIT;	
	};

}

void waitforkeyrelease() {
	
	/* auf Loslassen einer beliebigen Taste im Countdown-Zeitraum warten */
	cli();
	keyreleased0 = 0;
	keyreleased1 = 0;
	keypressed0 = 0;
	keypressed1 = 0;
	sei();
	while ((keyreleased0 | keyreleased1) == 0);

};

void set_door_on() {
	
	cli();
	SET_DOOR_ON(1 << DOOR_PB)
	SET_LED_ON(1 << LED_DOOR_PB)
	doorcntdwn = init_doorcntdwn;
	sei();

};

void set_door_off() {

	/* Abschaltung des Türöffners via Countdown erfolgt im Interrupt */
	cli();
	SET_DOOR_OFF(1 << DOOR_PB)
	SET_LED_OFF((1 << LED_CODE_PB) | (1 << LED_DOOR_PB))
	doorcntdwn = 0;
	sei();

};


uint8_t startup() {

    /* Startup: Alle LEDs blinken über Interrupt */

	cli();
	/*  Startup-Countdown initialisieren */
	startcntdwn = STARTUP_CNTDWN;
	ledcntdwn  = LED_CNTDWN_INIT;
	sei();
	
	/* Countdown auf Null abwarten; wird von Interruptroutine herabgezählt */
	while (startcntdwn > 0);	
	
	SET_LED_OFF((1 << LED_CODE_PB) | (1 << LED_PROG_PB) | (1 << LED_DOOR_PB))

	/* Wechsel in Status Codeeingabe */

	return STATE_READ_CODE;

};


uint8_t read_code() {

	/* Code entgegennehmen und Türöffner bei richtiger Eingabe aktivieren */
	codeptr = code;
	waitforkeyrelease();

	if (PROGKEYWASRELEASED) {
		/* Programmiertaster betätigt */
		return STATE_PROG_CODE;
	};

	cli();
	scancntdwn = SCAN_TIMER_INIT;	/* Countdown setzen */
	sei();
	
	while (scancntdwn > 0) {		/* nächste Code-Taste nur innerhalb des Countdown erwarten */

		/* Prüfung, ob laufende Taste richtig eingegeben wurde */	
		if ((*codeptr++ == keyreleased0) && 
	    	(*codeptr++ == keyreleased1)) {		/* lfd. Taste war richtig */
			SET_LED_ON(1 << LED_CODE_PB) 
			scancntdwn = SCAN_TIMER_INIT;		/* Countdown resetten */
		}
		else {								
			/* eingegebene Taste war falsch */
			SET_LED_OFF(1 << LED_CODE_PB)
			return STATE_READ_CODE;
		};

		/* Codeeingabe fehlerfrei am Ende angekommen? */
		if ((*codeptr == 0) && (*(codeptr+1) == 0)) {			
			return STATE_DOOR_ENABLED;
		};
	
		/* nächsten Tastendruck innerhalb des Countdown abwarten */
		cli();
		keyreleased0 = 0;
		keyreleased1 = 0;
		sei();
		while ((scancntdwn > 0) && (keyreleased0 | keyreleased1) == 0);
		
	}; /* while */

			
	/* Timeout bei der Codeeingabe oder falsche Taste */
	SET_LED_OFF(1 << LED_CODE_PB)
	return STATE_READ_CODE;

};

uint8_t door_enabled() {

	/* Türöffner einschalten */

	set_door_on();
	cli();
	keypressed0 = 0;
	keypressed1 = 0;
	sei();
	while((doorcntdwn > 0) && ((keypressed0 | keypressed1) == 0));
	set_door_off();
	if ((keypressed0 | keypressed1) > 0) waitforkeyrelease();	/* falls zum Abbruch Taste betätigt, diese verwerfen */
	return STATE_READ_CODE;

};

uint8_t prog_code() {

	/* Programmierung des Codes nach Drücken der Programmiertaste */

	SET_LED_ON(1 << LED_PROG_PB)

	waitforkeyrelease();

	if (PROGKEYWASRELEASED) {
		/* Wechsel in Programmiermodus für Türaktivierdauer, wenn Programmiertaste gedrückt wurde */
		return STATE_PROG_DOOR;
	};
	
	codeptr = code;

	/* Tastendrücke entgegennehmen und speichern */
	while(!PROGKEYWASRELEASED) {
		*codeptr++ = keyreleased0;
		*codeptr++ = keyreleased1;
		/* nur Tasten speichern, soweit Speicher ausreicht */
		if (codeptr > (code + sizeof(code) - 4)) break;
		waitforkeyrelease();
	};

	/* neuer Code wurde erfasst: Nullbytes am Codeende anfügen */
	*codeptr++ = 0;
	*codeptr = 0;

	/* neuen Code im EEPROM ablegen */
	cli();
	eeprom_write_block(code, eeprom_code, sizeof(code));
	sei();

	SET_LED_OFF(1 << LED_PROG_PB)
	return STATE_READ_CODE;	/* zurück in Codeeingabe-Modus */

};


uint8_t prog_door() {

	/* Einstellen der Türaktivierdauer nach wiederholtem Drücken des Programmiertaste ohne Codeeingabe */

	while(1) {
		waitforkeyrelease();
		/* Plus-Taste erhöht Aktivierdauer */
		if ((keyreleased0 & (1 << PROG_DOOR_PLUS))  > 0 && (init_doorcntdwn < 100) ) init_doorcntdwn++;
		/* Minus-Taste reduziert Aktivierdauer */
		if ((keyreleased0 & (1 << PROG_DOOR_MINUS)) > 0 && (init_doorcntdwn >   5) ) init_doorcntdwn--;
		 /* Türöffner zum Test aktivieren */
		if ((keyreleased0 & (1 << PROG_DOOR_TEST)) > 0) set_door_on();
		if (PROGKEYWASRELEASED) {
			/* geänderte Türaktivierdauer in EEPROM ablegen */
			cli();
			eeprom_write_byte(&eeprom_initdoorcntdwn, init_doorcntdwn);
			sei();
			break;
		};
		if ((keyreleased0 & ((1 << PROG_DOOR_PLUS) | (1 << PROG_DOOR_MINUS) | (1 << PROG_DOOR_TEST))) == 0) {
			/* undefinierte Taste wurde betätigt */
			/* veränderte Türaktivierdauer verwerfen, alten Wert aus EEPROM laden */
			init_doorcntdwn = eeprom_read_byte(&eeprom_initdoorcntdwn);
			break;
		};

	}; /* while */
	
	set_door_off();
	SET_LED_OFF(1 << LED_PROG_PB)
	return STATE_READ_CODE;

};

/* Array mit statusspezifischen Funktionen */
pFunc stateFunc[] = { startup, read_code, door_enabled, prog_code, prog_door };

int main() {

	cli();

	status = STATE_STARTUP;
	
	int_cntdwn = INT_CNTDWN_INIT;

	/* Pinports auf Eingang, die lt. Maske als Eingang vorgesehen sind */
	KEYBANK0_DDR = KEYBANK0_MASK;
	KEYBANK1_DDR = KEYBANK1_MASK;

	/* Pullup-Widerstände einschalten bei den lt. Maske benötigten Pins */
	KEYBANK0_PORT = (uint8_t) ~KEYBANK0_MASK;
	KEYBANK1_PORT = (uint8_t) ~KEYBANK1_MASK;

	#if (FORCE_USE_KEYPAD == 1)
	  usematrixkeypad = 1;
	#else
	  /* Pin zur Ermittlung, ob Keypad genutzt werden soll */
	  usematrixkeypad = ~KEYBANK0_PIN & (1 << KEYPAD_ENABLE_PB );
	#endif

	if (usematrixkeypad) {
		KEYBANK0_DDR = 0b01110000;
	};
	
	/* Status der Tasten */
	keystate0 = KEYBANK0_PIN;
	keyvertcnt0a = 0;
	keyvertcnt0b = 0;
	keypressed0 = 0;
	keyreleased0 = 0;

	/* LED-Portpins auf Ausgang */
	LED_DDR |= (1 << LED_CODE_PB) | (1 <<LED_PROG_PB) | (1 << LED_DOOR_PB);

	/* Türöffner-Portpin auf Ausgang */
	DOOR_DDR |= (1 << DOOR_PB);

	/* Türöffner definiert ausschalten */
	SET_DOOR_OFF(1 << DOOR_PB)

	/* Timer 0 */
	/* Unterscheidung nach ATMEGA und ATtiny */
	#ifdef TCCR0
	  TCCR0  = PRESCALER;
  	#endif
	#ifdef TCCR0B
	  TCCR0B = PRESCALER;
	#else
	#endif

	TIMSK = (1 << TOIE0);

	init_doorcntdwn = eeprom_read_byte(&eeprom_initdoorcntdwn);
	eeprom_read_block(code, eeprom_code, sizeof(code));

	/* Interrupts aktivieren */
	sei();

	while(1) {

		status = stateFunc[status]();

	};

	return 0;
}
