/*
 * Lcd.h
 *
 *  Created on: Jul 16, 2011
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

#ifndef LCD_H_
#define LCD_H_

#include <WProgram.h>
#include <avr/io.h>
#include <SoftwareSerial.h>


#define STRING_DATE "Date:"
#define STRING_TIME "Time:"
#define STRING_TEMP "Temp:"
#define STRING_PH	"pH  :"

#define LCD_CURSOR_POS_DATE_START	(LCD_LINE_1_START + 6)
#define LCD_CURSOR_POS_TIME_START	(LCD_LINE_2_START + 6)
#define LCD_CURSOR_POS_TEMP_START	(LCD_LINE_3_START + 6)
#define LCD_CURSOR_POS_PH_START		(LCD_LINE_4_START + 6)



#define LCD_CONTROL					0xFE
#define LCD_DISPLAY_ON				0x41
#define LCD_DISPLAY_OFF				0x42
#define LCD_SET_CURSOR_POS			0X45
#define LCD_HOME					0x46
#define LCD_CURSOR_UNDERLINE_ON		0x47
#define LCD_CURSOR_UNDERLINE_OFF	0x48
#define LCD_CURSOR_LEFT				0x49
#define LCD_CURSOR_RIGHT			0x4A
#define LCD_CURSOR_BLINK_ON			0x4B
#define LCD_CURSOR_BLINK_OFF		0x4C
#define	LCD_BACKSPACE				0x4E
#define LCD_CLEAR_SCREEN			0x51
#define LCD_SET_CONTRAST			0x52
#define LCD_SET_BRIGHTNESS			0x53
#define LCD_LOAD_CHARACTER			0x54
#define LCD_SHIFT_LEFT				0x55
#define LCD_SHIFT_RIGHT				0x56
#define LCD_SET_BAUD_RATE			0x61
#define LCD_SET_I2C_ADDRESS			0x62
#define LCD_DISPLAY_FIRMWARE		0x70
#define LCD_DISPLAY_BAUD_RATE		0x71
#define LCD_DISPLAY_I2C_ADDRESS		0x72

#define LCD_LINE_1_START			0x00
#define LCD_LINE_1_END				0x13
#define LCD_LINE_2_START			0x40
#define LCD_LINE_2_END				0x53
#define LCD_LINE_3_START			0x14
#define LCD_LINE_3_END				0x27
#define LCD_LINE_4_START			0x54
#define LCD_LINE_4_END				0x67

#define LCD_RX_PIN 3
#define LCD_TX_PIN 4

class Lcd
{
public:
	Lcd();
	void initialize();
	void updateTemp(double temp);
	void updatepH(double ph);
	void updateDate(uint8_t month, uint8_t day, uint8_t year);
	void updateTime(uint8_t hour, uint8_t min, uint8_t sec, boolean is12, boolean isPM);
	void home();
	void clear();
	void setBrightness(uint8_t brightness);
	void setContrast(uint8_t contrast);
	void setCursorPosition(uint8_t pos);
	void moveCursor(uint8_t direction);
	void enableDisplay(boolean b);
	void enableUnderline(boolean b);
	void enableBlink(boolean b);
	void shift(uint8_t direction);
	void showFirmware();
	void showBaudRate();
	void setBaudRate(uint8_t baud);
	void showI2CAddress();
	void setI2CAddress(uint8_t address);

protected:
	SoftwareSerial lcdPort;

	void sendCommand(uint8_t command);

};

extern Lcd LCD;
extern volatile uint8_t seconds;

#endif /* LCD_H_ */
