/*
    SPIFFS_Memory.cpp - A simple key-value based database implementation 
                    for Arduino based microcontrollers using SPIFFS memory
    Created by Aman Khatri, Aug 7, 2020
    Released into the public domain
*/

#include "Arduino.h"
#include "SPIFFS_Memory.h"

/**
 * Constructor of the class for SPIFFS memory
 */
SPIFFS_Memory::SPIFFS_Memory()
{
	_FILE_NAME = "/store.txt";
	_isInitiated = false;
}



// ****************** PRIVATE METHODS *************************

void SPIFFS_Memory::_print(String msg)
{
#ifdef DEBUG
	Serial.print("*ArduinoDb[SPIFFS]* ");
	Serial.println(msg);
#endif
}




int SPIFFS_Memory::_indexOfKey(String key)
{
	// Opening and reading the file contents
	File file = SPIFFS.open(_FILE_NAME, "r");

	if (!file)
	{
		_print("INDEX_OF_KEY operation failed");
		return -1;
	}
	else 
	{
		int keyIndex = 0;
		int keyByteIndex = 0;
		String keyStr = "";

		for (int i = 0 ; i < file.size() ; i++)
		{
			char currentChar = (char)file.read();

			if (currentChar == '>')
			{
				if (key.equals(keyStr))
				{
					// Checking if key active or not
					currentChar = (char)file.read();
					if (currentChar == '1')
					{
						file.close();
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

				keyStr = "";
			} else {
				keyStr = keyStr + String(currentChar);
			}

			if (currentChar == '\n')
			{
				keyIndex++;

				keyStr = "";
			}

			keyByteIndex++;
		}

		file.close();

		return -1;
	}
}




int8_t SPIFFS_Memory::_optimizeMemory(int spaceRequired, int fileSize, bool forceOptimize)
{
// 	FSInfo fs_info;
// SPIFFS.info(fs_info);
// float fileTotalKB = (float)fs_info.totalBytes; 
// float fileUsedKB = (float)fs_info.usedBytes; 
// float realFlashChipSize = (float)ESP.getFlashChipRealSize() / 1024.0 / 1024.0;
// Serial.print("    Actual size based on chip Id: "); Serial.print(realFlashChipSize); Serial.println(" MB ... given by (2^( \"Device\" - 1) / 8 / 1024");
// Serial.print("    Total KB: "); Serial.print(fileTotalKB); Serial.println(" KB");
// Serial.print("    Used KB: "); Serial.print(fileUsedKB); Serial.println(" KB");
// Serial.printf("    Block size: %lu\n", fs_info.blockSize);
// Serial.printf("    Page size: %lu\n", fs_info.pageSize);

	_print("Inside Optimize Memory");

	FSInfo fs_info;
	SPIFFS.info(fs_info);

	int totalAvailableBytes = (int)fs_info.totalBytes;

	// Only 80% space is usable
	// totalAvailableBytes = (totalAvailableBytes * 0.8);
	totalAvailableBytes = MAX_SPIFFS_SIZE;

	_print("Space Required (in bytes): " + String(spaceRequired));
	_print("Current used space (in bytes): " + String(fileSize));
	_print("Total available space (in bytes): " + String(totalAvailableBytes));

	// Checking if space availabe is sufficient or not
	if (((spaceRequired + fileSize) < totalAvailableBytes) && (!forceOptimize))
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

		File file = SPIFFS.open(_FILE_NAME, "r");

		if (!file)
		{
			_print("Optimization operation failed");
			return FAILURE;
		}
		else 
		{
			// Reading the whole data from the file
			String currentData = "";

			for (int i = 0 ; i < file.size() ; i++)
			{
				currentData = currentData + String((char)file.read());
			}

			// _print("Previous File Data:- \n" + currentData);

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

					if((char)temp[indexOfSep + 1] == '1')
					{
						newData = newData + temp + "\n";
					}

					temp = "";
				}
			}

			file.close();
			// _print("New File Data:- \n" + newData);

			// Writing new data to the file
			file = SPIFFS.open(_FILE_NAME, "w");

			if (!file)
			{
				_print("Optimization operation failed");
				return FAILURE;
			}
			else 
			{
				file.print(newData);
			}

			// Checking if space availabe after optimization is sufficient or not
			fileSize = file.size();

			if (((spaceRequired + fileSize) > totalAvailableBytes))
			{
				file.close();
				_print("Memory full, please delete some data");
				return MEM_FULL;
			}

			file.close();

			return SUCCESS;
		}
	}
	
}

// ************************************************************



// ****************** PUBLIC METHODS **************************
bool SPIFFS_Memory::begin()
{
	_print("Initializing the system");

	if (SPIFFS.begin())
	{
		_isInitiated = true;

		return SUCCESS;
	}
	else
	{
		_isInitiated = false;

		return FAILURE;
	}
}




bool SPIFFS_Memory::format()
{
	_print("Formatting the system");

	if (_isInitiated)
	{
		if (SPIFFS.format())
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




int8_t SPIFFS_Memory::optimize()
{
	return _optimizeMemory(0, 0, true);
}




String SPIFFS_Memory::get(String key, String defaultValue)
{
	if (_isInitiated)
	{
		// Opening and reading the file contents
		File file = SPIFFS.open(_FILE_NAME, "r");

		_print("Size: " + String(file.size()));

		if (!file)
		{
			_print("Get operation failed");
			return defaultValue;
		}
		else 
		{
			int keyIndex = _indexOfKey(key);

			if (keyIndex == -1)
			{
				return defaultValue;
			}
			else
			{
				String data = "";
				file.seek(keyIndex, SeekSet);
				char currentChar = (char)file.read();

				while (currentChar != '\n') 
				{
					data = data + currentChar;

					currentChar = (char)file.read();
				}

				_print(data);

				// int activeSepIdx = data.indexOf('>');

				// TODO: Structure to pass key and value maybe
				// Obtaining value from the key
				int seperatorIndex = data.indexOf(':');

				data = data.substring(++seperatorIndex);

				file.close();
				_print("Get operation successfull");

				return data;
			}
		}
	}
	else
	{
		_print("System not initiated");
	}
	
	_print("GET CALLED");
	return defaultValue;
}




String SPIFFS_Memory::getAll()
{
	if (_isInitiated)
	{
		File file = SPIFFS.open(_FILE_NAME, "r");

		if (!file) 
		{
			_print("GETALL operation failed");
			return "";
		}
		else
		{
			// Reading data from file
			String data = "";

			for (int i = 0 ; i < file.size() ; i++)
			{
				data = data + String((char)file.read());
			}

			file.close();
			return data;
		}
		
	}
	else
	{
		_print("System not initiated");
	}

	return "";
}




int8_t SPIFFS_Memory::insert(String key, String value)
{
	_print("INSERT CALLED");
	
	if (_isInitiated) 
	{
		// Opening/creating the file and writing the key, value pair into the file
		if (!SPIFFS.exists(_FILE_NAME))
		{
			// Create the File
			File file = SPIFFS.open(_FILE_NAME, "w");
			file.close();
		}

		File file = SPIFFS.open(_FILE_NAME, "r+");

		if (!file) 
		{
			_print("Insert operation failed");
			return FAILURE;
		}
		else 
		{
			String data = key + ">1:" + value + "\n";

			// Finding the kye index if key already exists
			int keyIndex = _indexOfKey(key);

			_print("Key index: " + String(keyIndex));

			if (keyIndex != -1) 
			{
				// Key already exists, remove that key
				remove(key);
				file.seek(keyIndex, SeekSet);
			}

			// Before writing to file performing optimizations if required
			int optimize_res = _optimizeMemory(data.length(), file.size(), false);

			if (optimize_res == FAILURE)
				return FAILURE;
			else if (optimize_res == MEM_FULL)
				return MEM_FULL;
			
			// Adding key value pair
			file.seek(0, SeekEnd);
			file.print(data);
			file.close();

			_print("Insert operation successfull");
			return SUCCESS;
		}
	}
	else
	{
		_print("System not initiated");
	}

	return FAILURE;
}




bool SPIFFS_Memory::remove(String key)
{
	if (_isInitiated)
	{
		File file = SPIFFS.open(_FILE_NAME, "r+");

		if (!file) 
		{
			_print("REMOVE operation failed");
			return FAILURE;
		}
		else 
		{
			// Finding the kye index if key already exists
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
				// Writing empty/space characters at that key
				int idx = keyIndex;
				file.seek(idx, SeekSet);
				char currentChar = (char)file.read();

				while (currentChar != '>') 
				{
					file.seek(idx, SeekSet);

					idx++;
					currentChar = (char)file.read();
				}

				file.print("0");

				file.close();

				_print("REMOVE operation successfull");

				return SUCCESS;
			}
		}
	}
	else
	{
		_print("System not initiated");
	}

	_print("REMOVE CALLED");

	return FAILURE;
}




bool SPIFFS_Memory::exists(String key)
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

// #ifdef ESP8266
// 		Serial.println("You're using ESP8266 based microcontroller");
// 	#endif

// 	#ifdef ESP32
// 		Serial.println("You're using ESP32 based microcontroller");
// 	#endif

// 	#ifdef AVR
// 		Serial.println("You're using AVR based microcontroller");
// 	#endif