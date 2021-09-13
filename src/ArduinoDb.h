/*
    ArduinoDb.h - A simple key-value based database implementation 
                    for Arduino based microcontrollers
    Created by Aman Khatri, Aug 7, 2020
    Released into the public domain
*/

#ifndef ArduinoDb_h
#define ArduinoDb_h

#include "Arduino.h"
#include "SPIFFS_Memory.h"
#include "EEPROM_Memory.h"

class ArduinoDb {
    private:
        byte _mode;     // 0- EEPROM, and 1- SPIFFS

        SPIFFS_Memory _SPIFFSMemory = SPIFFS_Memory();
		EEPROM_Memory _EEPROMMemory = EEPROM_Memory(0);


    public:
        /**
         * Constructor for initializing class object for using SPIFFS memory
         * @param null
         * @return null
         */
        ArduinoDb();

        /**
         * Constructor for initializing class object for using EEPROM memory
         * @param EEPROMSize size of the memory you want to use for Database storage(in bytes)
         * @return null
         */
        ArduinoDb(int EEPROMSize);

        /**
         * This method will handle the initialization of the library
         * Note- Must be called within setup only once
         * @return FAILURE is initialization fails
         * @return SUCCESS is initialization successful
         */
        bool begin();

        /**
         * This method will format the file system
         * @return FAILURE is format fails
         * @return SUCCESS is format successful
         */
        bool format();

        /**
         * Call this method to optimize the database forcefully
         * @param null
         * @return SUCCESS, if optimization task successful and space is available for new data
         * @return FAILURE, if optimization failed or space for new data is not available
         */
        int8_t optimize();

        /**
         * This method will return the value associated with key
         * @param key key for which value is required
         * @param defaultValue default value to return if key not found
         * @return value associated with key if found
         * @return defaultValue otherwise
         */
        String get(const String& key, const String& defaultValue);

        /**
         * This method will return all the stored key value pairs
         * @param null
         * @return String of all key value pairs
         */
        String getAll();

        /**
         * This method will insert the data into the database
         * @param key unique key for the value
         * @param value value associated with the key
         * @return SUCCESS if value inserted successfully
         * @return FAILURE is value insertion failed
         */
        int8_t insert(const String& key, const String& value);

        /**
         * This method will remove the key and associated value from db
         * @param key key to be removed
         * @return FAILURE if fails or key not found
         * @return SUCCESS if key removed successfully
         */
        bool remove(const String& key);

        /**
         * This method will tell weather the key exists or not in the database
         * @param key key to search for
         * @return SUCCESS if key found
         * @return FAILURE is key not found
         */
        bool exists(const String& key);
};

#endif