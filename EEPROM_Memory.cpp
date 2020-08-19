/*
    EEPROM_Memory.cpp - A simple key-value based database implementation 
                    for Arduino based microcontrollers using EEPROM memory
    Created by Aman Khatri, Aug 7, 2020
    Released into the public domain
*/

#include "Arduino.h"
#include "EEPROM_Memory.h"

/**
 * Constructor of the class for EEPROM memory
 */
EEPROM_Memory::EEPROM_Memory(int EEPROMSize)
{
	_EEPROM_SIZE = EEPROMSize;
	_isInitiated = false;
}



// ****************** PRIVATE METHODS *************************

void EEPROM_Memory::_print(String msg)
{
#ifdef DEBUG
	Serial.print("*ArduinoDb[EEPROM]* ");
	Serial.println(msg);
#endif
}




int EEPROM_Memory::_indexOfKey(String key)
{
	int fileSize = _getFilesize();
	int keyIndex = 0;
	int keyByteIndex = 0;
	String keyStr = "";

	for (int i = 0 ; i < fileSize ; i++)
	{
		char currentChar = (char)EEPROM.read(i);

		if (currentChar == '>')
		{
			if (key.equals(keyStr))
			{
				// Checking if key active or not
				i = i + 1;
				currentChar = (char)EEPROM.read(i);
				
				if (currentChar == '1')
				{
					_print("(inside indexOfKey) Key found at: " + String(keyByteIndex - keyStr.length()));
					// TODO: Maybe a structure for both
					return keyByteIndex - keyStr.length();
					return keyIndex;
				}
				else
				{
					keyByteIndex++;
				}
			}
		}
		else
		{
			keyStr = keyStr + String(currentChar);
		}

		if (currentChar == '\n')
		{
			keyIndex++;
			keyStr = "";
		}

		keyByteIndex++;
	}

	return -1;
}




int8_t EEPROM_Memory::_optimizeMemory(int spaceRequired, int fileSize, bool forceOptimize)
{	
	_print("Inside Optimize Memory");

	int totalAvailableBytes = _EEPROM_SIZE;

	_print("Space Required (in bytes): " + String(spaceRequired));
	_print("Current used space (in bytes): " + String(fileSize));
	_print("Total available space (in bytes): " + String(totalAvailableBytes));

	// Checking if space availabe is sufficient or not
	if (((spaceRequired + fileSize) < (totalAvailableBytes - 5)) && (!forceOptimize))
	{
		_print("Sufficient space available, optimization not required");
		return SUCCESS;
	}
	else
	{
		if (forceOptimize)
		{
			_print("Force optimizing memory...performing optimization");
		}
		else
		{
			_print("Space not available...performing optimization");
		}

		// Reading complete data from file
		String currentData = "";

		for (int i = 0 ; i < fileSize ; i++)
		{
			currentData = currentData + String((char)EEPROM.read(i));
		}

		// Removing un-indexed data that is not required
		String newData = "";
		String temp = "";
		char thisChar;

		for (int i = 0 ; i < currentData.length() ; i++)
		{
			thisChar = (char)currentData[i];

			if (thisChar != '\n')
			{
				temp = temp + String(thisChar);
			}
			else
			{
				int indexOfSep = temp.indexOf(">");

				if ((char)temp[indexOfSep + 1] == '1')
				{
					newData = newData + temp + "\n";
				}

				temp = "";
			}
		}

		_print("New File Data:- \n" + newData);

		// Formatting before writing new data
		format();

		// Writing new data to EEPROM
		int newDataIdx = 0;

		for (int i = 0 ; i <= newData.length() ; i++)
		{
			EEPROM.write(i, (char)newData[newDataIdx]);
			newDataIdx++;
		}

		if (EEPROM.commit())
		{
			_print("Write operation successful");
		} 
		else
		{
			_print("Write operation failed");
			return FAILURE;
		}

		// Checking if space availabe after optimization is sufficient or not
		fileSize = _getFilesize();

		if (((spaceRequired + fileSize) > (totalAvailableBytes - 5)))
		{
			_print("Memory full, please delete some data");
			return MEM_FULL;
		}

		return SUCCESS;
	}
	
}




int EEPROM_Memory::_getFilesize()
{
	char dataChars[3];
	int i = 0;

	for (i = 0 ; i < _EEPROM_SIZE ; i = i + 3)
	{
		dataChars[0] = (char)EEPROM.read(i);
		dataChars[1] = (char)EEPROM.read(i + 1);
		dataChars[2] = (char)EEPROM.read(i + 2);

		if ((dataChars[0] == '\n') && (dataChars[1] == '\n') && (dataChars[2] == '\n'))
		{
			if (i <= 0)
			{
				_print("File size:- " + String(i));
				return i;
			}

			// Reverse parsing for removing any extra characters
			for (int j = i ; j >= 0 ; j--)
			{
				if (((char)EEPROM.read(j) != '\n'))
				{
					_print("File size:- " + String(j + 1));
					return j + 1;
				}
			}
		}
	}

	_print("File size:- " + String(i));
	return i;
}

// ************************************************************



// ****************** PUBLIC METHODS **************************
bool EEPROM_Memory::begin()
{
    _print("Initializing the system");

    if ((_EEPROM_SIZE <= 0) || (_EEPROM_SIZE > MAX_EEPROM_SIZE))
    {
        _isInitiated = false;
        return FAILURE;
    }

    // Setting up EEPROM
    EEPROM.begin(_EEPROM_SIZE);

    _isInitiated = true;

    return SUCCESS;
}




bool EEPROM_Memory::format()
{
    _print("Formatting the system");

    if (_isInitiated)
	{
		// Write a 0 to all bytes of the EEPROM
		for (int i = 0; i < _EEPROM_SIZE; i++) 
		{
			EEPROM.write(i, '\n');
		}

		EEPROM.commit();

		return SUCCESS;
	}
	else
	{
		_print("System not initiated");
	}

	return FAILURE;
}




int8_t EEPROM_Memory::optimize()
{
	return _optimizeMemory(0, _getFilesize(), true);
}




String EEPROM_Memory::get(String key, String defaultValue)
{
	_print("GET CALLED");
	
    if (_isInitiated)
	{
		int keyIndex = _indexOfKey(key);

		if (keyIndex == -1)
		{
			return defaultValue;
		}
		else
		{
			String data = "";
			int i = keyIndex;

			char currentChar = (char)EEPROM.read(i);

			while (currentChar != '\n')
			{
				data = data + String(currentChar);
				i = i + 1;
				currentChar = (char)EEPROM.read(i);
			}

			_print(data);

			int seperatorIndex = data.indexOf(':');

			data = data.substring(++seperatorIndex);

			_print("Get operation successfull");

			return data;
		}
		
	}
	else
	{
		_print("System not initiated");
	}

	return defaultValue;
}




String EEPROM_Memory::getAll()
{
	_print("GETALL CALLED");
	
    if (_isInitiated)
	{
		String data = "";
		int fileSize = _getFilesize();

		for (int i = 0 ; i < fileSize ; i++)
		{
			data = data + String((char)EEPROM.read(i));
		}

		return data;
	}
	else
	{
		_print("System not initiated");
	}

	return "";
}




// TODO: Optimize/defrag memory before inserting data if memory is close to full
int8_t EEPROM_Memory::insert(String key, String value)
{
	_print("INSERT CALLED");

    if (_isInitiated)
	{
		String data = key + ">1:" + value + "\n";
		int fileSize = _getFilesize();

		// Finding the key index if key already exists
		int keyIndex = _indexOfKey(key);

		if (keyIndex != -1)
		{
			// Key exists, remove that first
			remove(key);
		}

		// Before writing to file performing optimizations if required
		int optimize_res = _optimizeMemory(data.length(), fileSize, false);

		if (optimize_res == FAILURE)
			return FAILURE;
		else if (optimize_res == MEM_FULL)
			return MEM_FULL;

		int dataIdx = 0;
		fileSize = _getFilesize();

		for (int i = fileSize ; i <= (fileSize + data.length()) ; i++)
		{
			EEPROM.write(i, (char)data[dataIdx]);
			dataIdx++;
		}

		if (EEPROM.commit())
		{
			_print("Write operation successful");
			return SUCCESS;
		} 
		else
		{
			_print("Write operation failed");
			return FAILURE;
		}
	}
	else
	{
		_print("System not initiated");
	}

	return FAILURE;
}




bool EEPROM_Memory::remove(String key)
{
	_print("REMOVE CALLED");
	
    if (_isInitiated)
	{
		// Searching for key index
		int keyIndex = _indexOfKey(key);

		_print(String(keyIndex));

		if (keyIndex == -1)
		{
			// Key does not exists
			_print("Key not found");
			return FAILURE;
		}
		else
		{
			// Key exists
			int idx = keyIndex;
			char currentChar = (char)EEPROM.read(idx);

			while (currentChar != '>')
			{
				idx++;
				currentChar = (char)EEPROM.read(idx);
			}

			EEPROM.write(idx + 1, '0');

			if (EEPROM.commit())
			{
				return SUCCESS;
			}
		}
		
	}
	else
	{
		_print("System not initiated");
	}

	return FAILURE;
}




bool EEPROM_Memory::exists(String key)
{
	_print("EXISTS CALLED");

	if (_isInitiated)
	{
		int idx = _indexOfKey(key);

		if (idx != -1)
		{
			return SUCCESS;
		}
		else
		{
			return FAILURE;
		}
	}
	else
	{
		_print("System not initiated");
	}

	return FAILURE;
}

// ************************************************************

// TODO: Testing

// #ifdef ESP8266
// 		Serial.println("You're using ESP8266 based microcontroller");
// 	#endif

// 	#ifdef ESP32
// 		Serial.println("You're using ESP32 based microcontroller");
// 	#endif

// 	#ifdef AVR
// 		Serial.println("You're using AVR based microcontroller");
// 	#endif