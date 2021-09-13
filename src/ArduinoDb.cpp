/*
    ArduinoDb.cpp - A simple key-value based database implementation 
                    for Arduino based microcontrollers
    Created by Aman Khatri, Aug 7, 2020
    Released into the public domain
*/

#include "Arduino.h"
#include "ArduinoDb.h"

/**
 * Constructor of the class for SPIFFS memory
 */
ArduinoDb::ArduinoDb()
{
	_mode = 1;
	_SPIFFSMemory = SPIFFS_Memory();
}




/**
 * Constructor of the class for EEPROM memory
 */
ArduinoDb::ArduinoDb(int EEPROMSize)
{
	_mode = 0;
	_EEPROMMemory = EEPROM_Memory(EEPROMSize);
}




// ****************** PUBLIC METHODS **************************
bool ArduinoDb::begin()
{
	if (_mode == 0)
	{
		return _EEPROMMemory.begin();
	}
	else if (_mode == 1)
	{
		return _SPIFFSMemory.begin();
	}
}




bool ArduinoDb::format()
{
	if (_mode == 0)
	{
		return _EEPROMMemory.format();
	}
	else if (_mode == 1)
	{
		return _SPIFFSMemory.format();
	}
}




int8_t ArduinoDb::optimize()
{
	if (_mode == 0)
	{
		return _EEPROMMemory.optimize();
	}
	else if (_mode == 1)
	{
		return _SPIFFSMemory.optimize();
	}
}




String ArduinoDb::get(String key, String defaultValue)
{	
	if (_mode == 0)
	{
		return _EEPROMMemory.get(key, defaultValue);
	}
	else if (_mode == 1)
	{
		return _SPIFFSMemory.get(key, defaultValue);
	}
}




String ArduinoDb::getAll()
{
	if (_mode == 0)
	{
		return _EEPROMMemory.getAll();
	}
	else if (_mode == 1)
	{
		return _SPIFFSMemory.getAll();
	}
}




// TODO: Optimize/defrag memory before inserting data if memory is close to full
int8_t ArduinoDb::insert(String key, String value)
{
	if (_mode == 0)
	{
		return _EEPROMMemory.insert(key, value);
	}
	else if (_mode == 1)
	{
		return _SPIFFSMemory.insert(key, value);
	}
}




bool ArduinoDb::remove(String key)
{
	if (_mode == 0)
	{
		return _EEPROMMemory.remove(key);
	}
	else if (_mode == 1)
	{
		return _SPIFFSMemory.remove(key);
	}
}




bool ArduinoDb::exists(String key)
{
	if (_mode == 0)
	{
		return _EEPROMMemory.exists(key);
	}
	else if (_mode == 1)
	{
		return _SPIFFSMemory.exists(key);
	}
}

// ***********************************************************