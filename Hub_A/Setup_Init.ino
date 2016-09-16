

void setupBluetooth()
{
  // Make sure Bluetooth is initially disconnected
  BTSerial.atTEST();
  
  // Read Mode switch and set the mode
  HubMode = digitalRead(MODE_SWITCH);
  if(HubMode)
  {
    BTSerial.atROLE('1');
    Serial.println(F("Entering Hub Mode"));
  }
  else
  {
    BTSerial.atROLE('0');
    Serial.println(F("Entering Config Mode"));
  }

  BTSerial.atRESET();
  digitalWrite(MODE_LED, HubMode);
}


void setupInitial()
{
  Serial.println(F("Getting Data From SD Card")); 
  // Get Hub ID from Hub_B
  sendSetupCommand("2", HubID);
  
  // Get Alert Phone Number from Hub_B
  sendSetupCommand("11", AlertPhone);
  
  // Get Portal Phone Number from Hub_B
  sendSetupCommand("13", PortalPhone);

  // Clean up Buffers again...
  clearAllBuffers();

  // Get Portal Frequency
  sendSetupCommand("15", Data);
  LoggingFreq = atoi(Data);
  memset(Data, '\0', DATA_SIZE);

  // Get Logging Frequency
  sendSetupCommand("17", Data);
  LoggingFreq = atoi(Data);
  memset(Data, '\0', DATA_SIZE);
  
  // Get Critical Temperature from Hub_B
  sendSetupCommand("23", Data);
  critTemp = atoi(Data);
  memset(Data, '\0', DATA_SIZE);
  
  // Get Critical Humidity from Hub_B
  sendSetupCommand("25", Data);
  critHum = atoi(Data);
  memset(Data, '\0', DATA_SIZE);
}


void getSetupSensor(byte num)
{
  // Send a command to Hub_B
  Serial.print(F("Sent: "));
  Serial.print(6);
  Serial.print(' ');
  Serial.println(num);
  
  SerialB.print(6);
  SerialB.print(' ');
  SerialB.println(num);
  SerialB.flush();

  delay(1000);
  
  // Read Response from Hub_B
  clearBuffer();
  byte bytesRead = SerialB.readBytesUntil('\n', Buffer, BUFF_SIZE-1);
  if(bytesRead > 0)
  {
    Serial.print(F("Received: "));
    Serial.println(Buffer);
  }
}


void setupSensors()
{
  Serial.println(F("Setting up Sensors..."));
  // Loop and get each sensor address
  for(byte i = 0; i < MAX_SENSORS; i++)
  {
    getSetupSensor(i);
    if(Buffer[0] != 'e')
    {
      // Add the Sensor to SensorList
      Sensor * newSensor = new Sensor();
      memcpy(newSensor->address, Buffer, ADDR_SIZE);
      newSensor->address[ADDR_SIZE] = '\0';

      SensorList.add(newSensor);

      Serial.print(F("Sensor "));
      Serial.print(i);
      Serial.print(F(": "));
      Serial.println(newSensor->address);
    }
    else
    {
      Serial.println("Done Reading Sensors from SD Card!");
      break;
    }
    
  }

}

void printSetup()
{
  Serial.println(F("~~~~~Hub Setup Settings~~~~~"));
  Serial.print(F("HubID: "));
  Serial.write(HubID, HUB_ID_SIZE);
  Serial.println();
  Serial.print(F("AlertPhone: "));
  Serial.println(AlertPhone);
  Serial.print(F("PortalPhone: "));
  Serial.println(PortalPhone);
  Serial.print(F("Critical Temperature: "));
  Serial.println(critTemp);
  Serial.print(F("Critical Humidity: "));
  Serial.println(critHum);
  Serial.print(F("Number of Sensors: "));
  Serial.println(SensorList.size());
  Serial.flush();
}







