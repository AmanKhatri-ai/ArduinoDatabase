/*
    EEPROM_Memory.h - A simple key-value based database implementation 
                    for Arduino based microcontrollers using EEPROM memory
    Created by Aman Khatri, Aug 7, 2020
    Released into the public domain
*/

#ifndef EEPROM_Memory_h
#define EEPROM_Memory_h

#include "Arduino.h"
#include <EEPROM.h>
#include "Config.h"

// Possible failure and success values
// #define FAILURE false
// #define SUCCESS true

class EEPROM_Memory 
{
    private:
        uint16_t _EEPROM_SIZE;
        bool _isInitiated;

        /**
         * This function will just print the message to the Serial if DEBUG is 1
         * @param msg The message to print in Serial
         */
        void _print(String msg);

        /**
         * This will return the index at which key is available
         * @param key the key whose index is to be searched
         * @return index of key if found
         * @return -1 if key index not found
         */
        int _indexOfKey(String key);

         /**
         * This method will perform memory optimization if required by checking the space needed for new data
         * @param spaceRequired variable containing required empty memory required, if that much memory is 
         *                      not available optimization will be performed (in bytes)
         * @param fileSize current total size used by file (in bytes)
         * @param forceOptimize set to true for optimizing memory even if space available
         * @return SUCCESS, if optimization task successful and space is available for new data
         * @return FAILURE, if optimization failed or space for new data is not available
         */
        int8_t _optimizeMemory(int spaceRequired, int fileSize, bool forceOptimize);

		/**
		 * To calculate the size occupied by EEPROM stored data in EEPROM memory 
		 * @param null
		 * @return number of bytes occupied by the data in EEPROM memory
		 */
        int _getFilesize();


    public:
        /**
         * Constructor for initializing class object for using EEPROM memory
         * @param EEPROMSize size of the memory you want to use for Database storage(in bytes)
         * @return null
         */
        EEPROM_Memory(int EEPROMSize);

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
        String get(String key, String defaultValue);

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
        int8_t insert(String key, String value);

        /**
         * This method will remove the key and associated value from db
         * @param key key to be removed
         * @return FAILURE if fails or key not found
         * @return SUCCESS if key removed successfully
         */
        bool remove(String key);

        /**
         * This method will tell weather the key exists or not in the database
         * @param key key to search for
         * @return SUCCESS if key found
         * @return FAILURE is key not found
         */
        bool exists(String key);

};

#endif