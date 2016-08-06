/* 
 * Name: main.c
 * Project: 4-Key-Keyboard(up/down/enter/escape) 
 * Author: Albert David(albert dot david at gmail)
 * Creation Date: 2016-08-07
 * This code is written for Digispark attiny85 board(http://digistump.com/products/1)
 * it turns digispark-attiny85-board to act as 4key hid device(key_up/key_dn/key_enter/key_escape)
*/

/*
 * many parts of this code is based on following two projects(Christian Starkjohann/Flip van den Berg)
 * Project: Thermostat based on AVR USB driver
 * Author: Christian Starkjohann
 * Creation Date: 2006-04-23
 * Tabsize: 4
 * Copyright: (c) 2006 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: Proprietary, free under certain conditions. See Documentation.
 * This Revision: $Id: main.c 537 2008-02-28 21:13:01Z cs $
 * Project: 4-Key-Keyboard
 * Author: Flip van den Berg - www.flipwork.nl
 * Creation Date: February 2010
 * Based on V-USB drivers from Objective Developments - http://www.obdev.at/products/vusb/index.html
 *
 */
/* ------------------------------------------------------------------------- */
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "usbdrv.h"
#include "oddebug.h"
/* ------------------------------------------------------------------------- */
#define BUTTON_PORT_B1 PORTB       /* PORTx - register for BUTTON 1 output */
#define BUTTON_PIN_B1 PINB         /* PINx - register for BUTTON 1 input */
#define BUTTON_BIT_B1 PB0 //1          /* bit for BUTTON 1 input/output */

#define BUTTON_PORT_B2 PORTB       /* PORTx - register for BUTTON 2 output */
#define BUTTON_PIN_B2 PINB         /* PINx - register for BUTTON 2 input */
#define BUTTON_BIT_B2 PB1 //4          /* bit for BUTTON 2 input/output */

#define BUTTON_PORT_B3 PORTB       /* PORTx - register for BUTTON 3 output */
#define BUTTON_PIN_B3 PINB         /* PINx - register for BUTTON 3 input */
#define BUTTON_BIT_B3 PB2 //5          /* bit for BUTTON 3 input/output */

#define BUTTON_PORT_B4 PORTB       /* PORTx - register for BUTTON 4 output */
#define BUTTON_PIN_B4 PINB         /* PINx - register for BUTTON 4 input */
#define BUTTON_BIT_B4 PB5 //3          /* bit for BUTTON 4 input/output */
/* ------------------------------------------------------------------------- */
static uchar    reportBuffer[8] = {0,0,0,0,0,0,0,0};    /* buffer for HID reports */
static uchar    idleRate;           /* in 4 ms units */
static uchar    newReport = 0;		/* current report */
static uchar    buttonState_B1 = 3;		/*  stores state of button 0 */
static uchar    buttonState_B2 = 3;		/*  stores state of button 1 */
static uchar    buttonState_B3 = 3;		/*  stores state of button 2 */
static uchar    buttonState_B4 = 3;		/*  stores state of button 3 */
static uchar    buttonChanged_B1;		
static uchar    buttonChanged_B2;		
static uchar    buttonChanged_B3;		
static uchar    buttonChanged_B4;		
static uchar	debounceTimeIsOver = 1;	/* for switch debouncing */
/* ------------------------------------------------------------------------- */
PROGMEM char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)	** Modifier Byte **
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)	** Reserved Byte **
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)	** LED Report **
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)	** LED Report Padding **
    0x95, 0x06,                    //   REPORT_COUNT (6)** here we define the maximum number of simultaneous keystrokes we can detect ** 
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)	** Key arrays (6 bytes) **
    0xc0                           // END_COLLECTION  
};
/* We use a simplifed keyboard report descriptor which does not support the
 * boot protocol. We don't allow setting status LEDs and we only allow one
 * simultaneous key press (except modifiers). We can therefore use short
 * 2 byte input reports.
 * The report descriptor has been created with usb.org's "HID Descriptor Tool"
 * which can be downloaded from http://www.usb.org/developers/hidpage/.
 * Redundant entries (such as LOGICAL_MINIMUM and USAGE_PAGE) have been omitted
 * for the second INPUT item.
 */
/* Keyboard usage values, see usb.org's HID-usage-tables document, chapter
 * 10 Keyboard/Keypad Page for more codes.
 */
#define KEY_UP_ARROW   0x52
#define KEY_DN_ARROW   0x51
#define KEY_ENTER      0x28
#define KEY_ESCAPE     0x29
//0x51	Keyboard DownArrow
//0x52	Keyboard UpArrow
//0x28	Keyboard Return (ENTER)
//0x29	Keyboard ESCAPE
//0x4F	Keyboard RightArrow
//0x50	Keyboard LeftArrow
/* ------------------------------------------------------------------------- */
static void timerPoll(void)
{
	static unsigned int timerCnt;
	if(TIFR & (1 << TOV1))
	{
		TIFR = (1 << TOV1); /* clear overflow */
		if(++timerCnt >= 5)
		{       
		// 5/63 sec delay for switch debouncing
		timerCnt = 0;
		debounceTimeIsOver = 1; 
		}
	}
}
/* ------------------------------------------------------------------------- */
static void buildReport(void)
{
	uchar key; 
	if(newReport == 0)
	{	
		if (buttonChanged_B1 == 1)
		{
	        	if (buttonState_B1 != 0)
			{ // if button 1 is released
				key = 0; //button released event
			} 
			else 
			{ //if button 1 is pressed
				key = KEY_UP_ARROW;//30; // key = '1'
	 	   	}
			buttonChanged_B1 = 0;
			reportBuffer[2] = key;

		}
		if (buttonChanged_B2 == 1)
		{
        		if (buttonState_B2 != 0)
			{ // if button 2 is pressed
				key = 0; //button released event
			} 
			else 
			{
				key = KEY_DN_ARROW;//31;  // key = '2'
			}
			buttonChanged_B2 = 0;
    			reportBuffer[3] = key;
    		}
		if(buttonChanged_B3 == 1)
		{
        		if (buttonState_B3 != 0)
			{ // if button 3 is pressed
				key = 0; //button released event
			} 
			else
			{
				key = KEY_ENTER;//32; // key = '3'
			}
			buttonChanged_B3 = 0;
			reportBuffer[4] = key;
	    	}
		if(buttonChanged_B4 == 1)
		{
        		if (buttonState_B4 != 0)
			{ // if button 4 is pressed
				key = 0; //button released event
			} 
			else
			{
				key = KEY_ESCAPE;//33;  // key = '4'
    			}
			buttonChanged_B4 = 0;
    			reportBuffer[5] = key;
    		}
		newReport = 1;; //if no button has changed, the previous report will be sent
	}
}
/* ------------------------------------------------------------------------- */
static void checkButtonChange(void) {
	
	uchar tempButtonValue_B1 = bit_is_set(BUTTON_PIN_B1, BUTTON_BIT_B1); //status of switch is stored in tempButtonValue 
	uchar tempButtonValue_B2 = bit_is_set(BUTTON_PIN_B2, BUTTON_BIT_B2); //status of switch is stored in tempButtonValue 
	uchar tempButtonValue_B3 = bit_is_set(BUTTON_PIN_B3, BUTTON_BIT_B3);  //status of switch is stored in tempButtonValue 
	uchar tempButtonValue_B4 = bit_is_set(BUTTON_PIN_B4, BUTTON_BIT_B4);  //status of switch is stored in tempButtonValue 

	if (tempButtonValue_B1 != buttonState_B1){ //if status has changed
		buttonState_B1 = tempButtonValue_B1;	// change buttonState to new state
		debounceTimeIsOver = 0;	// debounce timer starts
		newReport = 0; // initiate new report 
		buttonChanged_B1 = 1;
	}
	if (tempButtonValue_B2 != buttonState_B2){ //if status has changed
		buttonState_B2 = tempButtonValue_B2;	// change buttonState to new state
		debounceTimeIsOver = 0;	// debounce timer starts
		newReport = 0; // initiate new report 
		buttonChanged_B2 = 1;
	}
	if (tempButtonValue_B3 != buttonState_B3){ //if status has changed
		buttonState_B3 = tempButtonValue_B3;	// change buttonState to new state
		debounceTimeIsOver = 0;	// debounce timer starts
		newReport = 0; // initiate new report 
		buttonChanged_B3 = 1;
	}
	if (tempButtonValue_B4 != buttonState_B4){ //if status has changed
		buttonState_B4 = tempButtonValue_B4;	// change buttonState to new state
		debounceTimeIsOver = 0;	// debounce timer starts
		newReport = 0; // initiate new report 
		buttonChanged_B4 = 1;
	}
}
/* ------------------------------------------------------------------------- */
static void timerInit(void)
{
	/* select clock: 16.5M/1k -> overflow rate = 16.5M/256k = 62.94 Hz */
	TCCR1 = 0x0b;
}
/* ------------------------------------------------------------------------- */
/* ------------------------ interface to USB driver ------------------------ */
/* ------------------------------------------------------------------------- */
uchar usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;
	usbMsgPtr = reportBuffer;
	if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)
	{    	/* class request type */
		if(rq->bRequest == USBRQ_HID_GET_REPORT)
		{  	/* wValue: ReportType (highbyte), ReportID (lowbyte) */
			/* we only have one report type, so don't look at wValue */
			buildReport();
			return sizeof(reportBuffer);
		}
		else if(rq->bRequest == USBRQ_HID_GET_IDLE)
		{
			usbMsgPtr = &idleRate;
			return 1;
		}
		else if(rq->bRequest == USBRQ_HID_SET_IDLE)
		{
			idleRate = rq->wValue.bytes[1];
		}
	}
	else
	{
		/* no vendor specific requests implemented */
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
/* ------------------------ Oscillator Calibration ------------------------- */
/* ------------------------------------------------------------------------- */
/* Calibrate the RC oscillator to 8.25 MHz. The core clock of 16.5 MHz is
 * derived from the 66 MHz peripheral clock by dividing. Our timing reference
 * is the Start Of Frame signal (a single SE0 bit) available immediately after
 * a USB RESET. We first do a binary search for the OSCCAL value and then
 * optimize this value with a neighboorhod search.
 * This algorithm may also be used to calibrate the RC oscillator directly to
 * 12 MHz (no PLL involved, can therefore be used on almost ALL AVRs), but this
 * is wide outside the spec for the OSCCAL value and the required precision for
 * the 12 MHz clock! Use the RC oscillator calibrated to 12 MHz for
 * experimental purposes only!
 */
/* ------------------------------------------------------------------------- */
#define abs(x) ((x) > 0 ? (x) : (-x))
void    calibrateOscillator(void)
{
	uchar       step = 128;
	uchar       trialValue = 0, optimumValue;
	int         x, optimumDev, targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);

	/* do a binary search: */
	do{
		OSCCAL = trialValue + step;
		x = usbMeasureFrameLength();    /* proportional to current real frequency */
		if(x < targetValue)             /* frequency still too low */
		trialValue += step;
		step >>= 1;
		}while(step > 0);
		/* We have a precision of +/- 1 for optimum OSCCAL here */
		/* now do a neighborhood search for optimum value */
		optimumValue = trialValue;
		optimumDev = x; /* this is certainly far away from optimum */
		for(OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++){
		x = usbMeasureFrameLength() - targetValue;
		if(x < 0)
		x = -x;
		if(x < optimumDev)
		{
			optimumDev = x;
			optimumValue = OSCCAL;
		}
	}
	OSCCAL = optimumValue;
}
/* ------------------------------------------------------------------------- */
// Called by V-USB after device reset
void usbEventResetReady(void)
{
	calibrateOscillator();
	//eeprom_write_byte(0, OSCCAL); 
}
/* ------------------------------------------------------------------------- */
/* --------------------------------- main ---------------------------------- */
/* ------------------------------------------------------------------------- */
int main(void)
{
	uchar   i;
	odDebugInit();
	usbInit();
	usbDeviceDisconnect();  //enforce re-enumeration, do this while interrupts are disabled!
	i = 0;
	while(--i)
	{             // fake USB disconnect for > 250 ms
		wdt_reset();
		_delay_ms(1);
	}
	usbDeviceConnect();
	wdt_enable(WDTO_2S);

	// turn on internal pull-up resistor for the switches
	BUTTON_PORT_B1 |= _BV(BUTTON_BIT_B1);
	BUTTON_PORT_B2 |= _BV(BUTTON_BIT_B2);
	BUTTON_PORT_B3 |= _BV(BUTTON_BIT_B3);
	BUTTON_PORT_B4 |= _BV(BUTTON_BIT_B4);

	timerInit();
	sei();
	for(;;) //main event loop
	{    
		wdt_reset();
		usbPoll();
		if (debounceTimeIsOver == 1)
		{
		checkButtonChange();
		}
		if(usbInterruptIsReady() && newReport == 0) //we can send another report
		{ 
			buildReport();
			usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
		}
		timerPoll();
	}
	return 0;
}
/* ------------------------------------------------------------------------- */



