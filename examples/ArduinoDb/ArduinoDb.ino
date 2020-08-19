#include "ArduinoDb.h"

ArduinoDb arduinoDb;
ArduinoDb arduinoDbEEPROM(1024);

void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println("Serial ready");

  if (arduinoDb.begin()) {
    Serial.println("Initialization successful");
  } else {
    Serial.println("Initialization failed");
  }

  if (arduinoDbEEPROM.begin()) {
    Serial.println("Initialization successful");
  } else {
    Serial.println("Initialization failed");
  }
}

void loop() {
	if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    String cmd = "";
    int sepIdx = input.indexOf(",");
    cmd = input.substring(0, sepIdx);
    input = input.substring(++sepIdx);
    String key = "";
    String val= "";

    if (cmd.equals("insert"))
    {
      sepIdx = input.indexOf(",");
      key = input.substring(0, sepIdx);
      val = input.substring(++sepIdx);
      insertData(key, val);
    } 
    else if (cmd.equals("insert_eeprom"))
    {
      sepIdx = input.indexOf(",");
      key = input.substring(0, sepIdx);
      val = input.substring(++sepIdx);
      insertDataEEPROM(key, val);
    } 
    else if (cmd.equals("remove"))
    {
      key = input;
      removeData(key);
    }
    else if (cmd.equals("remove_eeprom"))
    {
      key = input;
      removeDataEEPROM(key);
    }
    else if (cmd.equals("get"))
    {
      key = input;
      getData(key);
    }
    else if (cmd.equals("get_eeprom"))
    {
      key = input;
      getDataEEPROM(key);
    }
    else if (cmd.equals("get_all")) 
    {
      getAllData();
    }
    else if (cmd.equals("get_all_eeprom")) 
    {
      getAllDataEEPROM();
    }
    else if (cmd.equals("exist"))
    {
      key = input;
      existData(key);
    }
    else if (cmd.equals("exist_eeprom"))
    {
      key = input;
      existDataEEPROM(key);
    }
    else if (cmd.equals("format"))
    {
      formatData();
    }
    else if (cmd.equals("format_eeprom"))
    {
      formatDataEEPROM();
    }
    else if (cmd.equals("optimize"))
    {
      optimizeMemory();
    }
    else if (cmd.equals("optimize_eeprom"))
    {
      optimizeMemoryEEPROM();
    }
    else
    {
      Serial.println("Invalid command");
    }
    
//    int keyIndex = arduinoDb._indexOfKey(input);
//
//    Serial.print(input + ": ");
//    Serial.println(String(keyIndex));
//    Serial.println(arduinoDb.get(input, "Default"));
	}
} 



void insertData(String key, String val) 
{
  Serial.println();
  Serial.println("********** Insert data(SPIFFS) **********");
  int res = arduinoDb.insert(key, val);
  
  if (res == SUCCESS) 
  {
    Serial.println("Insertion successfull");
  }
  else if (res == FAILURE)
  {
    Serial.println("Insertion failed");
  }
  else if (res == MEM_FULL)
  {
    Serial.println("Memory full");
  }
}



void insertDataEEPROM(String key, String val) 
{
  Serial.println();
  Serial.println("********** Insert data(EEPROM) **********");
  int res = arduinoDbEEPROM.insert(key, val);
  Serial.println(res);
  if (res == SUCCESS) 
  {
    Serial.println("Insertion successfull");
  }
  else if (res == FAILURE)
  {
    Serial.println("Insertion failed");
  }
  else if (res == MEM_FULL)
  {
    Serial.println("Memory full");
  }
}



void removeData(String key)
{
  Serial.println();
  Serial.println("********** Remove data(SPIFFS) **********");
  if (arduinoDb.remove(key)) 
  {
    Serial.println("Remove successfull");
  }
  else
  {
    Serial.println("Remove failed");
  }
}



void removeDataEEPROM(String key)
{
  Serial.println();
  Serial.println("********** Remove data(EEPROM) **********");
  if (arduinoDbEEPROM.remove(key)) 
  {
    Serial.println("Remove successfull");
  }
  else
  {
    Serial.println("Remove failed");
  }
}



void getData(String key)
{
  Serial.println();
  Serial.println("********** Get data(SPIFFS) **********");
  String val = arduinoDb.get(key, "-1");

  if (val == "-1")
  {
    Serial.println("Key not found");
  }
  else
  {
    Serial.println(val);
  }
}



void getDataEEPROM(String key)
{
  Serial.println();
  Serial.println("********** Get data(EEPROM) **********");
  String val = arduinoDbEEPROM.get(key, "-1");

  if (val == "-1")
  {
    Serial.println("Key not found");
  }
  else
  {
    Serial.println(val);
  }
}



void existData(String key) 
{
  Serial.println();
  Serial.println("********** Exist data(SPIFFS) **********");
  if (arduinoDb.exists(key))
  {
    Serial.println("Key exists");
  }
  else
  {
    Serial.println("Key not exists");
  }
}



void existDataEEPROM(String key) 
{
  Serial.println();
  Serial.println("********** Exist data(EEPROM) **********");
  if (arduinoDbEEPROM.exists(key))
  {
    Serial.println("Key exists");
  }
  else
  {
    Serial.println("Key not exists");
  }
}



void formatData() 
{
  Serial.println();
  Serial.println("********** Format data(SPIFFS) **********");
  if (arduinoDb.format())
  {
    Serial.println("Format successfull");
  } 
  else
  {
    Serial.println("Format failed");
  }
}



void formatDataEEPROM() 
{
  Serial.println();
  Serial.println("********** Format data(EEPROM) **********");
  if (arduinoDbEEPROM.format())
  {
    Serial.println("Format successfull");
  } 
  else
  {
    Serial.println("Format failed");
  }
}



void getAllData()
{
  Serial.println();
  Serial.println("********** Get all data(SPIFFS) **********");
  String data = arduinoDb.getAll();
  Serial.println(data);
}



void getAllDataEEPROM()
{
  Serial.println();
  Serial.println("********** Get all data(EEPROM) **********");
  String data = arduinoDbEEPROM.getAll();
  Serial.println(data);
}



void optimizeMemory()
{
  Serial.println();
  Serial.println("********** Optimize Memory(SPIFFS) **********");
  if (arduinoDb.optimize())
  {
    Serial.println("Optimization successful");
  }
  else
  {
    Serial.println("Optimization failed");
  }
}



void optimizeMemoryEEPROM()
{
  Serial.println();
  Serial.println("********** Optimize Memory EEPROM(EEPROM) **********");
  if (arduinoDbEEPROM.optimize())
  {
    Serial.println("Optimization successful");
  }
  else
  {
    Serial.println("Optimization failed");
  }
}
