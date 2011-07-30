/*
 * TemperatureProbe.h
 *
 *  Created on: Jul 23, 2011
 *      Author: tom
 */

#ifndef TEMPERATUREPROBE_H_
#define TEMPERATUREPROBE_H_

#include <WProgram.h>
#include <avr/io.h>

#define TEMP_PIN			0
#define FEEDBACK_PIN		1

#define TEMP_SAMPLE_SIZE 	10
#define TEMP_V_REF			3.32

class TemperatureProbe
{
public:
	TemperatureProbe();
	void initialize();
	void sample();
	double getLastValue();
	double getAverageValue();

private:
	volatile uint8_t index;
	volatile double sampleList[TEMP_SAMPLE_SIZE];
	volatile double currentSample;
	volatile double averageSample;
	volatile boolean filled;

};

extern TemperatureProbe TEMP;

#endif /* TEMPERATUREPROBE_H_ */
