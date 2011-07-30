/*
 * PhStamp.cpp
 *
 *  Created on: Jul 23, 2011
 *      Author: tom
 *
 * Copyright (c) 2011 Thomas M. Sasala.  All right reserved.
 *
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 * To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/
 * or send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 */

#include "PhStamp.h"

/** Create instance of class */
PhStamp PH = PhStamp();

/**
 * Constructor
 */
PhStamp::PhStamp()
{
	phProbe = SoftwareSerial(PH_RX_PIN, PH_TX_PIN);
	samples = 0;
	errors = 0;

	index = 0;
	averageSample = 0;
	currentSample = 0;
	filled = false;

} // end constructor

/**
 * Initialize pH
 */
void PhStamp::initialize()
{
	Serial.println("pH probe: initializing...");

	// Select the serial port for listening
	phProbe.listen();

	// Initialize serial
	phProbe.begin(PH_BAUD_RATE);

	// Turn on LED
	phProbe.print(PH_CMD_ENABLE_LED);
	delay(1000); // delay 1 second - from website; don't know why

	Serial.println("pH probe: initialization complete");

} // end initialize


/**
 * Samples pH
 */
void PhStamp::sample(double temp)
{
	uint8_t holding = 0;
	uint8_t i = 0;

//	Serial.println("\nListening to pH Serial port");

	// Select the serial port for listening
	phProbe.listen();

	// Send current temp
	phProbe.print( temp, 2);
	phProbe.print("\r");

	// Ask for a single sample
//	Serial.print("Asking for sample: ");
//	Serial.println( samples, DEC );

	phProbe.print("r\r");
	samples++;

	// per datasheet, 410ms delay w/ LED; 110ms w/o LED
	delay(500); // delay for processing and to send data (per datasheet)

	if( phProbe.available() > 3 )
	{
		holding = phProbe.available();
//		Serial.print("Sample Received.  Bytes available: ");
//		Serial.println(holding, DEC);

		for(i=0; i < holding; i++)
		{
			//Read data from serial buffer
			phProbeBuffer[i]= phProbe.read();
			if(phProbeBuffer[i] == 0x0d )
			{
				phProbeBuffer[i] = 0;
			}
		}

		// TODO: Need to provide some checks here before calling
		currentSample = atof((char *)&phProbeBuffer);

		// TODO: Ensure to ensure atof returned a real value


		// Print sample
//		Serial.print("Sample ");
//		Serial.print( samples, DEC );
//		Serial.print(" - pH: ");
//		Serial.println( (char *)&phProbeBuffer );
//		Serial.println( currentSample, 2 );

		// Compute average value
		sampleList[index++] = currentSample;
		averageSample = 0;
		if( filled )
		{
			// Compute new average
			for(int i=0; i<PH_SAMPLE_SIZE; i++)
			{
				averageSample += sampleList[i];
			}
			averageSample /= PH_SAMPLE_SIZE;
		}
		else
		{
			for(int i=0; i<index; i++)
			{
				averageSample += sampleList[i];
			}
			averageSample /= index;
		}
//		Serial.print("pH: ");
//		Serial.println( averageSample, 2 );

		if( index == PH_SAMPLE_SIZE )
		{
			index = 0;
			filled = true;
		}

	}
	else
	{
		errors++;
		Serial.print("** Error in Sample ");
		Serial.print( samples, DEC );
		Serial.print(", Error ");
		Serial.print( errors, DEC );
		Serial.print(": bytes available - ");
		Serial.println(phProbe.available(), DEC);
	}

}

char *PhStamp::getBuffer()
{
	return (char *)&phProbeBuffer;
}


double PhStamp::getLastValue()
{
	return currentSample;
}

double PhStamp::getAverageValue()
{
	return averageSample;
}

 // end sample
