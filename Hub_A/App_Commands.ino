/***** CASE 1 *****/
void factoryReset()
{
  
}

/***** CASE 2 *****/
void getHubID()
{
  BTSerial.write(HubID, HUB_ID_SIZE);
  BTSerial.write('\n');
}

/***** CASE 3 *****/
void setHubID()
{
  sendCommand("3");
  if(Buffer[0] == 'd')
    memcpy(HubID, Data, HUB_ID_SIZE);
  else
    checkErrorCode();
}

/***** CASE 4 *****/
void getNumSensors()
{
  BTSerial.println(SensorList.size());
}

/***** CASE 5 *****/
void getSensorList()
{
  if(SensorList.size() == 0)
    return;

  for(byte i = 0; i < SensorList.size(); i++)
  {
    Sensor * s = SensorList.get(i);
    BTSerial.println(s->address);
  }
}

/***** CASE 6 *****/
void getSensorID()
{
  // Not Implemented Yet
}

/***** CASE 7 *****/
void setSensorID()
{
  // Not Implemented Yet
}

/***** CASE 8 *****/
void addSensor()
{
  Sensor * newSensor = new Sensor();
  memcpy(newSensor->address, Data, ADDR_SIZE);
  newSensor->address[ADDR_SIZE] = '\0';

  
  // Send the new Sensor info to be saved in SD card
  SensorList.add(newSensor);
  SerialB.write("8");
  SerialB.write(" ");
  SerialB.write(newSensor->address);
  SerialB.write("\n");
  SerialB.flush();
  SerialB.listen();
  
  delay(1000);
  
  clearBuffer();
  // Get reply back from Hub_B
  byte bytesRead = SerialB.readBytesUntil('\n', Buffer, BUFF_SIZE-1);
  if(bytesRead > 0)
    Serial.println(Buffer);
  
  BTSerial.listen();
  
  if(Buffer[0] == 'd')
    BTSerial.println(F("Sensor Added!"));
  else
    checkErrorCode();
}

/***** CASE 9 *****/
void removeSensor()
{
  // Check if there are any sensors to remove
  if(SensorList.size() == 0)
  {
    Serial.println(F("No Sensors to remove!"));
    return;
  }

  // Convert the parameter into a number
  int num = atoi(Data);

  // Check if vaild
  if(num > SensorList.size())
  {
    Serial.println(F("Invalid Sensor Number!"));
    return;
  }

  // Remove sensor from the list
  SensorList.remove(num);

  // Send command to clear the Sensor File in the SD Card
  sendGetCommand(10);

  // Send Commands to Re-add all Sensors to the SD card file
  for(byte i = 0; i < SensorList.size(); i++)
  {
    Sensor * currSensor = SensorList.get(i);
    SerialB.write("8");
    SerialB.write(" ");
    SerialB.write(currSensor->address);
    SerialB.write("\n");
    SerialB.flush();

    delay(1000);
    
    clearBuffer();
    byte bytesRead = SerialB.readBytesUntil('\n', Buffer, BUFF_SIZE-1);
    if(bytesRead > 0)
      Serial.println(Buffer);
    
  }

}

/***** CASE 10 *****/
void removeAllSensors()
{
  // Clear off all sensors from SensorList
  SensorList.clear(); 
  
  // Send command to clear the Sensor File in the SD Card
  sendGetCommand(10);
}

/***** CASE 11 *****/
void getAlertPhone()
{
  BTSerial.println(AlertPhone);
}

/***** CASE 12 *****/
void setAlertPhone()
{
  sendCommand("12");
  if(Buffer[0] == 'd')
    memcpy(AlertPhone, Data, PHONE_SIZE);
  else
    checkErrorCode();
}

/***** CASE 13 *****/
void getPortalPhone()
{
  BTSerial.println(PortalPhone);
}

/***** CASE 14 *****/
void setPortalPhone()
{
  sendCommand("14");
  if(Buffer[0] == 'd')
    memcpy(PortalPhone, Data, PHONE_SIZE);
  else
    checkErrorCode();
}

/***** CASE 15 *****/
void getPortalFreq()
{
  BTSerial.println(PortalFreq);
}

/***** CASE 16 *****/
void setPortalFreq()
{
  sendCommand("16");
  
  if(Buffer[0] == 'd')
    PortalFreq = atoi(Data);
  else
    checkErrorCode();
}

/***** CASE 17 *****/
void getLoggingFreq()
{
  BTSerial.println(LoggingFreq);
}

/***** CASE 18 *****/
void setLoggingFreq()
{
  sendCommand("18");
  
  if(Buffer[0] == 'd')
    PortalFreq = atoi(Data);
  else
    checkErrorCode();
}

/***** CASE 19 *****/
void getHubTime()
{
  sendGetCommand(19);
  BTSerial.println(Buffer);
  BTSerial.flush();
}

/***** CASE 20 *****/
void setHubTime()
{
  sendCommand("20");

  if(Buffer[0] == 'd')
    BTSerial.println(F("Time Set!"));
  else
    checkErrorCode();
}

/***** CASE 21 *****/
void getHubDate()
{
  sendGetCommand(21);
  BTSerial.println(Buffer);
  BTSerial.flush();
}

/***** CASE 22 *****/
void setHubDate()
{
  sendCommand("22");
  
  if(Buffer[0] == 'd')
    BTSerial.println(F("Date Set!"));
  else
    checkErrorCode();
}

/***** CASE 23 *****/
void getCritTemp()
{
  BTSerial.println(critTemp);
}

/***** CASE 24 *****/
void setCritTemp()
{
  sendCommand("24");

  if(Buffer[0] == 'd')
    critTemp = atoi(Data);
  else
    checkErrorCode();
}

/***** CASE 25 *****/
void getCritHum()
{
  BTSerial.println(critHum);
}

/***** CASE 26 *****/
void setCritHum()
{
  sendCommand("26");
  
  if(Buffer[0] == 'd')
    critHum = atoi(Data);
  else
    checkErrorCode();
}

/***** CASE 27 *****/
void getData()
{
  // Send a command to Hub_B
  Serial.print(27);
  Serial.print(' ');
  Serial.println(Data);
  Serial.flush();
  
  SerialB.print(27);
  SerialB.print(' ');
  SerialB.println(Data);
  SerialB.flush();

  delay(1000);

  while(SerialB.available())
  {
    char c = SerialB.read();
    BTSerial.print(c);
    Serial.print(c);
  }
  
}







