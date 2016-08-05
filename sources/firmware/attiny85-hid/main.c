/* Name: main.c
 * Project: Thermostat based on AVR USB driver
 * Author: Christian Starkjohann
 * Creation Date: 2006-04-23
 * Tabsize: 4
 * Copyright: (c) 2006 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: Proprietary, free under certain conditions. See Documentation.
 * This Revision: $Id: main.c 537 2008-02-28 21:13:01Z cs $
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
/*
Pin assignment: Digispark attiny85 board(http://digistump.com/products/1)
PB0 = key input (active low with pull-up)
PB1 = LED output (active high)
PB3 = analog input (ADC3)
PB3, PB4 = USB data lines
*/
#define BIT_LED 1 //4
#define BIT_KEY 00
#define UTIL_BIN4(x)        (uchar)((0##x & 01000)/64 + (0##x & 0100)/16 + (0##x & 010)/4 + (0##x & 1))
#define UTIL_BIN8(hi, lo)   (uchar)(UTIL_BIN4(hi) * 16 + UTIL_BIN4(lo))
#ifndef NULL
#define NULL    ((void *)0)
#endif
/* ------------------------------------------------------------------------- */
static uchar    reportBuffer[2];    /* buffer for HID reports */
static uchar    idleRate;           /* in 4 ms units */
static uchar    adcPending;
static uchar    isRecording;
static uchar    valueBuffer[16];
static uchar    *nextDigit;
/* ------------------------------------------------------------------------- */
PROGMEM char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = { /* USB report descriptor */
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    // USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    // USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    // USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    // LOGICAL_MINIMUM (0)
    0x25, 0x01,                    // LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    // REPORT_SIZE (1)
    0x95, 0x08,                    // REPORT_COUNT (8)
    0x81, 0x02,                    // INPUT (Data,Var,Abs)
    0x95, 0x01,                    // REPORT_COUNT (1)
    0x75, 0x08,                    // REPORT_SIZE (8)
    0x25, 0x65,                    // LOGICAL_MAXIMUM (101)
    0x19, 0x00,                    // USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    // USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    // INPUT (Data,Ary,Abs)
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
#define MOD_CONTROL_LEFT    (1<<0)
#define MOD_SHIFT_LEFT      (1<<1)
#define MOD_ALT_LEFT        (1<<2)
#define MOD_GUI_LEFT        (1<<3)
#define MOD_CONTROL_RIGHT   (1<<4)
#define MOD_SHIFT_RIGHT     (1<<5)
#define MOD_ALT_RIGHT       (1<<6)
#define MOD_GUI_RIGHT       (1<<7)
#define KEY_1       30
#define KEY_2       31
#define KEY_3       32
#define KEY_4       33
#define KEY_5       34
#define KEY_6       35
#define KEY_7       36
#define KEY_8       37
#define KEY_9       38
#define KEY_0       39
#define KEY_RETURN  40
/* ------------------------------------------------------------------------- */
static void buildReport(void)
{
	uchar   key = 0;
	if(nextDigit != NULL)
	{
		key = *nextDigit;
	}
	reportBuffer[0] = 0;    /* no modifiers */
	reportBuffer[1] = key;
}
/* ------------------------------------------------------------------------- */
static void evaluateADC(unsigned int value)
{
	uchar   digit;
	value += value + (value >> 1);  /* value = value * 2.5 for output in mV */
	nextDigit = &valueBuffer[sizeof(valueBuffer)];
	*--nextDigit = 0xff;/* terminate with 0xff */
	*--nextDigit = 0;
	*--nextDigit = KEY_RETURN;
	do
	{
		digit = value % 10;
		value /= 10;
		*--nextDigit = 0;
		if(digit == 0)
		{
			*--nextDigit = KEY_0;
		}
		else
		{
			*--nextDigit = KEY_1 - 1 + digit;
		}
	}while(value != 0);
}
/* ------------------------------------------------------------------------- */
static void setIsRecording(uchar newValue)
{
	isRecording = newValue;
	if(isRecording)
	{
		PORTB |= 1 << BIT_LED;      /* LED on */
	}
	else
	{
		PORTB &= ~(1 << BIT_LED);   /* LED off */
	}
}
/* ------------------------------------------------------------------------- */
static void keyPoll(void)
{
	static uchar    keyMirror;
	uchar           key;
	key = PINB & (1 << BIT_KEY);
	if(keyMirror != key) /* status changed */
	{   	
		keyMirror = key;
		if(!key) /* key was pressed */
		{       
			setIsRecording(!isRecording);
		}
	}
}
/* ------------------------------------------------------------------------- */
static void adcPoll(void)
{
	if(adcPending && !(ADCSRA & (1 << ADSC)))
	{
		adcPending = 0;
		evaluateADC(ADC);
	}
}
/* ------------------------------------------------------------------------- */
static void timerPoll(void)
{
	static uchar timerCnt;
	if(TIFR & (1 << TOV1))
	{
		TIFR = (1 << TOV1); /* clear overflow */
		keyPoll();
		if(++timerCnt >= 63)
		{       /* ~ 1 second interval */
			timerCnt = 0;
			if(isRecording)
			{
				adcPending = 1;
				ADCSRA |= (1 << ADSC);/* start next conversion */
			}
		}
	}
}
/* ------------------------------------------------------------------------- */
static void timerInit(void)
{
	/* select clock: 16.5M/1k -> overflow rate = 16.5M/256k = 62.94 Hz */
	TCCR1 = 0x0b;
}
/* ------------------------------------------------------------------------- */
static void adcInit(void)
{
	ADMUX = UTIL_BIN8(1001, 0011);  /* Vref=2.56V, measure ADC0 */
	ADCSRA = UTIL_BIN8(1000, 0111); /* enable ADC, not free running, interrupt disable, rate = 1/128 */
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
}
/* ------------------------------------------------------------------------- */
/* --------------------------------- main ---------------------------------- */
/* ------------------------------------------------------------------------- */
int main(void)
{
	uchar   i;
	odDebugInit();
	usbInit();
	usbDeviceDisconnect();
	for(i=0;i<20;i++)
	{  /* 300 ms disconnect */
		_delay_ms(15);
	}
	usbDeviceConnect();
	DDRB |= 1 << BIT_LED;   /* output for LED */
	PORTB |= 1 << BIT_KEY;  /* pull-up on key input */
	wdt_enable(WDTO_1S);
	timerInit();
	adcInit();
	sei();
	for(;;)
	{    /* main event loop */
		wdt_reset();
		usbPoll();
		if(usbInterruptIsReady() && nextDigit != NULL){ /* we can send another key */
		buildReport();
		usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
		if(*++nextDigit == 0xff)    /* this was terminator character */
		nextDigit = NULL;
		}
		timerPoll();
		adcPoll();
	}
	return 0;
}
/* ------------------------------------------------------------------------- */



