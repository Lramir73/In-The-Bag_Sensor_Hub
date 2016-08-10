/***** CASE 1 *****/

/***** CASE 2 *****/
void getHubID()
{
  BTSerial.write(HubID, HUB_ID_SIZE);
  BTSerial.write('\n');
}

/***** CASE 3 *****/
void setHubID()
{
  sendCommand("22");
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
  
}

/***** CASE 7 *****/
void setSensorID()
{
  
}

/***** CASE 8 *****/
void addSensor()
{
  Sensor * newSensor = new Sensor();
  memcpy(newSensor->address, Data, ADDR_SIZE);
  newSensor->address[ADDR_SIZE] = '\0';

  SensorList.add(newSensor);
  SerialB.write("8");
  SerialB.write(" ");
  SerialB.write("0");
  SerialB.write(" ");
  SerialB.write(newSensor->address);
  SerialB.flush();


  // Send TBD Command to Hub_B
}

/***** CASE 9 *****/
void removeSensor()
{
  if(SensorList.size() == 0)
    return;
  for(byte i = 0; i < SensorList.size(); i++)
  {
    Sensor * currSensor = SensorList.get(i);
    if(memcmp(Data, currSensor->address, ADDR_SIZE) == 0)
    {
      SensorList.remove(i);
    }   
  }
  // Send TBD Command to Hub_B
}

/***** CASE 10 *****/
void removeAllSensors()
{
  SensorList.clear(); 
  // Send TBD Command to Hub_B
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
  sendCommand("10");
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
  sendCommand("14");
  
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
  sendCommand("16");
  
  if(Buffer[0] == 'd')
    PortalFreq = atoi(Data);
  else
    checkErrorCode();
}

/***** CASE 19 *****/
void getHubTime()
{
  sendGetCommand(2);
  BTSerial.println(Buffer);
  BTSerial.flush();
}

/***** CASE 20 *****/
void setHubTime()
{
  sendCommand("1");

  if(Buffer[0] == 'd')
    BTSerial.println(F("Time Set!"));
  else
    checkErrorCode();
}

/***** CASE 21 *****/
void getHubDate()
{
  sendGetCommand(4);
  BTSerial.println(Buffer);
  BTSerial.flush();
}

/***** CASE 22 *****/
void setHubDate()
{
  sendCommand("3");
  
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
  sendCommand("18");

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
  sendCommand("20");
  
  if(Buffer[0] == 'd')
    critHum = atoi(Data);
  else
    checkErrorCode();
}

/***** CASE 27 *****/
void getData()
{
  // Send a command to Hub_B
  Serial.print(7);
  Serial.print(' ');
  Serial.println(Data);
  Serial.flush();
  
  SerialB.print(7);
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







