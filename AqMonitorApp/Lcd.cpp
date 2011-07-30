/*
 * Lcd.cpp
 *
 *  Created on: Jul 23, 2011
 *      Author: tom
 */

#include "Lcd.h"

/** Create instance */
Lcd LCD = Lcd();

/**
 * Constructor
 */
Lcd::Lcd()
{
	lcdPort = SoftwareSerial(LCD_RX_PIN,LCD_TX_PIN);
}

/**
 * Initializes the display
 *
 */
void Lcd::initialize()
{
	Serial.println("Initializing LCD");

	// Select lcdPort. don't think we need this since we're always transmitting, but it doesn't hurt
	lcdPort.listen();

	// Initialize serial
	lcdPort.begin(9600);

	// Initialize LCD
	enableDisplay( false );
	clear();
	enableUnderline( false );
	enableBlink( false );

	// Set up screen
	setCursorPosition(LCD_LINE_1_START);
	lcdPort.print(STRING_DATE);

	setCursorPosition(LCD_LINE_2_START);
	lcdPort.print(STRING_TIME);

	setCursorPosition(LCD_LINE_3_START);
	lcdPort.print(STRING_TEMP);

	setCursorPosition(LCD_LINE_4_START);
	lcdPort.print(STRING_PH);

	// Turn screen on
	enableDisplay( true );

	Serial.println("LCD Initialization Complete.");

} // end initialize



/**
 * Updates display with a new temperature
 */
void Lcd::updateTemp(double temp)
{
	setCursorPosition(LCD_CURSOR_POS_TEMP_START);

	// Send temp to LCD
	lcdPort.print( temp, 2 );

} // end updateTemp


/**
 * Updates display with a new pH
 */
void Lcd::updatepH(double ph)
{
	// Set cursor location
	setCursorPosition(LCD_CURSOR_POS_PH_START);

	// Send ph to LCD
	lcdPort.print( ph, 2 );

} // end updatepH


/**
 * Updates display with a new time
 */
void Lcd::updateTime(uint8_t hour, uint8_t min, uint8_t sec, boolean is12, boolean isPM)
{
	// if stored time is different from read time, update.  Else, return

	setCursorPosition(LCD_CURSOR_POS_TIME_START);
	if( hour < 10 )
	{
		lcdPort.print( "0" );
	}
	lcdPort.print( hour, DEC );
	lcdPort.print( ":" );
	if( min < 10 )
	{
		lcdPort.print( "0" );
	}
	lcdPort.print( min, DEC );
	lcdPort.print( ":" );
	if( sec < 10 )
	{
		lcdPort.print( "0" );
	}
	lcdPort.print( sec, DEC );

	if( is12 )
	{
		if( isPM )
		{
			lcdPort.print(" PM");
		}
		else
		{
			lcdPort.print(" AM");
		}
	}
	else
	{
		lcdPort.print("   ");  // blank AM/PM if it was there; only need to do once...
	}

} // end updateTime

/**
 * Updates display with new date
 */
void Lcd::updateDate(uint8_t month, uint8_t day, uint8_t year)
{
	// if stored date is different from read date, update.  Else, return

	setCursorPosition(LCD_CURSOR_POS_DATE_START);

	// Send date to LCD
	if( month < 10 )
	{
		lcdPort.print( "0" );
	}
	lcdPort.print( month, DEC );
	lcdPort.print( "/" );
	if( day < 10 )
	{
		lcdPort.print( "0" );
	}
	lcdPort.print( day, DEC );
	lcdPort.print( "/20" );
	if( year < 10 )
	{
		lcdPort.print( "0" );
	}
	lcdPort.print( year, DEC );

} // end updateDate




/**
 * Moves cursor to home position
 */
void Lcd::home()
{
	sendCommand(LCD_HOME);
}

/**
 * Clears the screen
 */
void Lcd::clear()
{
	sendCommand(LCD_CLEAR_SCREEN);
}

/**
 * Sets the display brightness
 *
 */
void Lcd::setBrightness(uint8_t brightness)
{
	sendCommand(LCD_SET_BRIGHTNESS);
	lcdPort.print( brightness, BYTE);
}

/**
 * Sets the display contrast
 */
void Lcd::setContrast(uint8_t contrast)
{
	sendCommand(LCD_SET_CONTRAST);
	lcdPort.print( contrast, BYTE);
}

/**
 * Sets the cursor's position
 *
 */
void Lcd::setCursorPosition(uint8_t pos)
{
	sendCommand(LCD_SET_CURSOR_POS);
	lcdPort.print( pos );
}

/**
 * Moves cursor left or right
 *
 */
void Lcd::moveCursor(uint8_t direction)
{
	if(direction == LCD_CURSOR_LEFT || direction == LCD_CURSOR_RIGHT )
	{
		sendCommand( direction );
	}
}

/**
 * Turns display on or off
 *
 */
void Lcd::enableDisplay(boolean b)
{
	if( b == true )
	{
		sendCommand(LCD_DISPLAY_ON);
	}
	else
	{
		sendCommand(LCD_DISPLAY_OFF);
	}
}

/**
 * Enables cursor underline
 *
 */
void Lcd::enableUnderline(boolean b)
{
	if( b == true )
	{
		sendCommand(LCD_CURSOR_UNDERLINE_ON);
	}
	else
	{
		sendCommand(LCD_CURSOR_UNDERLINE_OFF);
	}

} // end enableUnderline


/**
 * Enables cursor blink
 *
 */
void Lcd::enableBlink(boolean b)
{
	if( b == true )
	{
		sendCommand(LCD_CURSOR_BLINK_ON);
	}
	else
	{
		sendCommand(LCD_CURSOR_BLINK_OFF);
	}

} // end enableBlink

/**
 * Shifts the display the specified direction
 */
void Lcd::shift(uint8_t direction)
{
	if(direction == LCD_SHIFT_LEFT || direction == LCD_SHIFT_RIGHT )
	{
		sendCommand( direction );
	}
}

/**
 * Shows the display firmware version
 */
void Lcd::showFirmware()
{
	sendCommand(LCD_DISPLAY_FIRMWARE);
}

/**
 * Shows the display's baudrate
 */
void Lcd::showBaudRate()
{
	sendCommand(LCD_DISPLAY_BAUD_RATE);

}

/**
 * Sets the display's baudrate
 */
void Lcd::setBaudRate(uint8_t baud)
{
	sendCommand(LCD_SET_BAUD_RATE);
	lcdPort.print( baud, BYTE );
}

/**
 * Shows the I2C address
 */
void Lcd::showI2CAddress()
{
	sendCommand(LCD_DISPLAY_I2C_ADDRESS);
}

/**
 * Sets the I2C address
 */
void Lcd::setI2CAddress(uint8_t address)
{
	sendCommand(LCD_SET_I2C_ADDRESS);
	lcdPort.print( address );
}

/**
 * Sends a command to LCD
 */
void Lcd::sendCommand(uint8_t command)
{
	// Send control sequence
	lcdPort.print(LCD_CONTROL, BYTE);
	lcdPort.print(command);
}


