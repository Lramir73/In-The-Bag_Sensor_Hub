/***** CASE 1 *****/

/***** CASE 2 *****/
void getHubID()
{
  BTSerial.println(HubID);
}

/***** CASE 3 *****/
void setHubID()
{
  sendCommand("22");
  if(Buffer[0] == 'd')
    memcpy(HubID, Data, HUB_ID_SIZE);
  else
    BTSerial.println(F("Command Failed."));
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
    BTSerial.print(s->address);
  }
}

/***** CASE 6 *****/
/***** CASE 7 *****/


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
  BTSerial.print(AlertPhone);
}

/***** CASE 12 *****/
void setAlertPhone()
{
  sendCommand("12");
  if(Buffer[0] == 'd')
    memcpy(AlertPhone, Data, PHONE_SIZE);
  else
    BTSerial.println(F("Command Failed."));
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
    BTSerial.println(F("Command Failed."));
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
    BTSerial.println(F("Command Failed."));
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
    BTSerial.println(F("Command Failed."));
}

/***** CASE 19 *****/
void getHubTime()
{
  sendCommand("2");
  BTSerial.println(Buffer);
}

/***** CASE 20 *****/
void setHubTime()
{
  sendCommand("1");
}




