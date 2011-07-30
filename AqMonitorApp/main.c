/*
 * main.c
 *
 *  Created on: Jul 15, 2011
 *      Author: tom
 *
 *
 *      Monitor consists of 3 sensors, LCD, and switch input.
 *      pH probe, temperature probe, and real-time clock.
 *
 * Copyright (c) 2011 Thomas M. Sasala.  All right reserved.
 *
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 * To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/
 * or send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 *
 */

#include "AqMonitorApp.h"
void sample();
void initialize();
void processCommand();
int SerialReadPosInt();

/**
 * Main routine
 */
int main(void)
{
	boolean sampleReady = false;
	uint8_t seconds = 255;

	// Initialize libraries
	init();

	// Initialize hardware serial port
	Serial.begin(115200);

	// Print start as FYI
	Serial.println("PROGRAM START");

	// Initialize hardware and modules
	initialize();

	while(1)
	{
		// Check serial port for cmd
		if(Serial.available())
		{
			processCommand();
		}

		// Keep reading clock until seconds change
		while(!sampleReady)
		{
			RTC.readClock();
			if( seconds != RTC.getSeconds() )
			{
				seconds = RTC.getSeconds();
				sampleReady = true;
			}
		}

		// Sample values
		sample();

		// Update Display
		LCD.updatepH( PH.getAverageValue() );
		LCD.updateTemp( TEMP.getAverageValue() );
		LCD.updateTime( RTC.getHours(), RTC.getMinutes(), RTC.getSeconds(), RTC.is12hour(), RTC.isPM() );
		LCD.updateDate( RTC.getMonth(), RTC.getDate(), RTC.getYear() );

		sampleReady = false;

	}

	// never get here, but...
	Serial.println("PROGRAM END");

	// Just in case
	while(1);

} // end main

/**
 * Main initialization routine.  Inits LCD, Temp, pH, and RTC.
 *
 */
void initialize()
{
	TEMP.initialize();
	PH.initialize();
	LCD.initialize();
	RTC.initialize();
}

/**
 * Samples values from ph and Temp
 */
void sample()
{
	// Sample temp
	TEMP.sample();

	// Read ph with temp compensation
	PH.sample( TEMP.getAverageValue() );

} // end sample

/**
 * Processes command from serial port
 */
void processCommand()
{
	if(!Serial.available()) { return; }

	char command = Serial.read();
	int in,in2;
	switch(command)
	{

		case 'H':
		case 'h':
			in=SerialReadPosInt();
			RTC.setHours(in);
			RTC.setClock();
			Serial.print("Setting hours to: ");
			Serial.println(in);
			break;
		case 'I':
		case 'i':
			in=SerialReadPosInt();
			RTC.setMinutes(in);
			RTC.setClock();
			Serial.print("Setting minutes to: ");
			Serial.println(in);
			break;
		case 'S':
		case 's':
			in=SerialReadPosInt();
			RTC.setSeconds(in);
			RTC.setClock();
			Serial.print("Setting seconds to: ");
			Serial.println(in);
			break;
		case 'Y':
		case 'y':
			in=SerialReadPosInt();
			RTC.setYear(in);
			RTC.setClock();
			Serial.print("Setting year to: ");
			Serial.println(in);
			break;
		case 'M':
		case 'm':
			in=SerialReadPosInt();
			RTC.setMonth(in);
			RTC.setClock();
			Serial.print("Setting month to: ");
			Serial.println(in);
			break;
		case 'D':
		case 'd':
			in=SerialReadPosInt();
			RTC.setDate(in);
			RTC.setClock();
			Serial.print("Setting date to: ");
			Serial.println(in);
			break;
		case 'W':
			Serial.print("Day of week is: ");
			Serial.println((int) RTC.getDayOfWeek());
			break;
		case 'w':
			in=SerialReadPosInt();
			RTC.setDayOfWeek(in);
			RTC.setClock();
			Serial.print("Setting day of week to: ");
			Serial.println(in);
			break;

		case 't':
		case 'T':
			if(RTC.is12hour())
			{
				RTC.switchTo24h();
				Serial.println("Switching to 24-hour clock.");
			}
			else
			{
				RTC.switchTo12h();
				Serial.println("Switching to 12-hour clock.");
			}
			RTC.setClock();
			break;

		case 'A':
		case 'a':
			if(RTC.is12hour())
			{
				RTC.setAM();
				RTC.setClock();
				Serial.println("Set AM.");
			}
			else
			{
				Serial.println("(Set hours only in 24-hour mode.)");
			}
			break;

		case 'P':
		case 'p':
			if(RTC.is12hour())
			{
				RTC.setPM();
				RTC.setClock();
				Serial.println("Set PM.");
			}
			else
			{
				Serial.println("(Set hours only in 24-hour mode.)");
			}
			break;

		case 'q':
			RTC.sqwEnable(RTC.SQW_1Hz);
			Serial.println("Square wave output set to 1Hz");
			break;
		case 'Q':
			RTC.sqwDisable(0);
			Serial.println("Square wave output disabled (low)");
			break;

		case 'z':
			RTC.start();
			Serial.println("Clock oscillator started.");
			break;
		case 'Z':
			RTC.stop();
			Serial.println("Clock oscillator stopped.");
			break;

		case '>':
			in=SerialReadPosInt();
			in2=SerialReadPosInt();
			RTC.writeData(in, in2);
			Serial.print("Write to register ");
			Serial.print(in);
			Serial.print(" the value ");
			Serial.println(in2);
			break;
		case '<':
			in=SerialReadPosInt();
			in2=RTC.readData(in);
			Serial.print("Read from register ");
			Serial.print(in);
			Serial.print(" the value ");
			Serial.println(in2);
			break;

		default:
			Serial.println("Unknown command. Try these:");
			Serial.println(" h## - set Hours d## - set Date");
			Serial.println(" i## - set mInutes m## - set Month");
			Serial.println(" s## - set Seconds y## - set Year");
			Serial.println(" w## - set arbitrary day of Week");
			Serial.println(" t - toggle 24-hour mode");
			Serial.println(" a - set AM p - set PM");
			Serial.println();
			Serial.println(" z - start clock Z - stop clock");
			Serial.println(" q - SQW/OUT = 1Hz Q - stop SQW/OUT");
			Serial.println();
			Serial.println(" >##,### - write to register ## the value ###");
			Serial.println(" <## - read the value in register ##");

	}//switch on command

}

//read in numeric characters until something else
//or no more data is available on serial.
int SerialReadPosInt()
{
	uint16_t i = 0;
	boolean done=false;
	while(Serial.available() && !done)
	{
		uint8_t c = Serial.read();
		if (c >= '0' && c <='9')
		{
			i = i * 10 + (c-'0');
		}
		else
		{
			done = true;
		}
	}
	return i;
}



extern "C" void __cxa_pure_virtual()
{
	cli();
	for (;;);
}

