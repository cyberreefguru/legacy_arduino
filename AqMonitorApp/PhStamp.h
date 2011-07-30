/*
 * PhStamp.h
 *
 *  Created on: Jul 23, 2011
 *      Author: tom
 */

#ifndef PHSTAMP_H_
#define PHSTAMP_H_

#include <WProgram.h>
#include <avr/io.h>
#include <SoftwareSerial.h>
#include <Wire.h>


#define PH_RX_PIN 6
#define PH_TX_PIN 7
#define PH_BAUD_RATE 38400

#define PH_CMD_SINGLE_SAMPLE "r\r"
#define PH_CMD_CONTINUOUS_SAMPLE "c\r"
#define PH_CMD_END_SAMPLE "e\r"
#define PH_CMD_ENABLE_LED "l1\r"
#define PH_CMD_DISABLE_LED "l0\r"

#define PH_SAMPLE_SIZE 	5

class PhStamp
{
public:
	PhStamp();
	void initialize();
	void sample(double temp);
	char* getBuffer();
	double getLastValue();
	double getAverageValue();


private:
	TwoWire test;
	SoftwareSerial phProbe;
	uint8_t phProbeBuffer[15];

	volatile uint16_t samples;
	volatile uint16_t errors;

	volatile uint8_t index;
	volatile double sampleList[PH_SAMPLE_SIZE];
	volatile double currentSample;
	volatile double averageSample;
	volatile boolean filled;


};

extern PhStamp PH;


#endif /* PHSTAMP_H_ */
