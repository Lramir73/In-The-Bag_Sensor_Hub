

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
  sendSetupCommand("23", HubID);
  
  // Get Alert Phone Number from Hub_B
  sendSetupCommand("13", AlertPhone);
  
  // Get Portal Phone Number from Hub_B
  sendSetupCommand("11", PortalPhone);

  // Clean up Buffers again...
  clearAllBuffers();

  // Get Portal Frequency
  sendSetupCommand("15", Data);
  LoggingFreq = atoi(Data);
  memset(Data, '0', DATA_SIZE);

  // Get Logging Frequency
  sendSetupCommand("17", Data);
  LoggingFreq = atoi(Data);
  memset(Data, '0', DATA_SIZE);
  
  // Get Critical Temperature from Hub_B
  sendSetupCommand("19", Data);
  critTemp = atoi(Data);
  memset(Data, '0', DATA_SIZE);
  
  // Get Critical Humidity from Hub_B
  sendSetupCommand("21", Data);
  critTemp = atoi(Data);
  memset(Data, '0', DATA_SIZE);
}


void setupSensors()
{
  // Get Sensor MAC Addresses from Hub_B
  sendSetupCommand("9 0", Data);

  // Add the Sensor to SensorList
  Sensor * newSensor = new Sensor();
  memcpy(newSensor->address, Data, ADDR_SIZE);
  newSensor->address[ADDR_SIZE] = '\0';

  SensorList.add(newSensor);
  Serial.print("Sensor 0: ");
  Serial.println(newSensor->address);
}





