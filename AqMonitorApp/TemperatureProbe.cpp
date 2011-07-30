/*
 * TemperatureProbe.cpp
 *
 *  Created on: Jul 23, 2011
 *      Author: tom
 */

#include "TemperatureProbe.h"

/** Instance */
TemperatureProbe TEMP = TemperatureProbe();


TemperatureProbe::TemperatureProbe()
{
	index = 0;
	averageSample = 0;
	currentSample = 0;
	filled = false;

} // end constructor

/**
 * Initializes the probe
 *
 */
void TemperatureProbe::initialize()
{
	 pinMode( TEMP_PIN, INPUT ); // set LM34 temp sensor pin as an input
	 pinMode( FEEDBACK_PIN, INPUT ); // Ref voltage feedback

	 analogReference(EXTERNAL); // set the analog reference to external

} // end initialize

/**
 * Samples the temperature from probe
 */
void TemperatureProbe::sample()
{
	uint16_t sample;
	float voltage;

	// Do sampling
	analogRead(TEMP_PIN); // throw away value
	delay(20);
	sample = analogRead(TEMP_PIN); // actual value
	delay(20);

	voltage = sample * TEMP_V_REF;
	voltage /= 1024;

//	currentSample = ( TEMP_V_REF * sample * 100.0) / 1024.0;
	currentSample = voltage*100;

//	Serial.print("Voltage: ");
//	Serial.print(voltage);
//	Serial.print(" Sample: ");
//	Serial.print(sample);
//	Serial.print(" Temp: ");
//	Serial.println(currentSample);


	// Compute average value
	sampleList[index++] = currentSample;
	averageSample = 0;
	if( filled )
	{
		// Compute new average
		for(int i=0; i<TEMP_SAMPLE_SIZE; i++)
		{
			averageSample += sampleList[i];
		}
		averageSample /= TEMP_SAMPLE_SIZE;
	}
	else
	{
		for(int i=0; i<index; i++)
		{
			averageSample += sampleList[i];
		}
		averageSample /= index;
	}
//	Serial.print("Temp: ");
//	Serial.println( averageSample, 2 );

	if( index == TEMP_SAMPLE_SIZE )
	{
		index = 0;
		filled = true;
	}
}

/**
 * Returns the last temp sampled
 */
double TemperatureProbe::getLastValue()
{
	return currentSample;
}


/**
 * Returns the average value sampled over the last TEMP_SAMPLE_SIZE
 */
double TemperatureProbe::getAverageValue()
{
	return averageSample;
}




