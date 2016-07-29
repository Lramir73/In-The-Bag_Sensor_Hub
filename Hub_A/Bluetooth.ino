void BluetoothParser(void)
{
  byte ByteCount =  BTSerial.readBytesUntil('\n', Buffer, BUFF_SIZE-1);
  
  if(ByteCount  > 0) 
  {
    // Parse Command and Data parameters
    strcpy(Command,strtok(Buffer," "));             
    strcpy(Data,strtok(NULL,";"));

    // Protect from overflowing buffers
    Command[CMD_SIZE-1] = '\0';
    Data[DATA_SIZE-1] = '\0';

    // Convert command into a byte
    byte cmd = atoi(Command);

    // Check if Busy status needs to be set
    if(cmd > MIN_CMD && cmd < MAX_CMD)
    digitalWrite(BUSY_LED, HIGH);
      
    // Execute Command
    switch(cmd)
    {
      case 2: // Get Hub ID
        getHubID();
        break;
      case 3: // Set Hub ID
        setHubID();
        break;
      case 4:
        getNumSensors();
        break;
      case 5: // Get List of Sensors
        getSensorList();
        break;
      case 6:
        break;
      case 7:
        break;
      case 8: // Add a Sensor
        addSensor();
        break;
      case 9: // Remove a Sensor
        removeSensor();
        break;
      case 10: // Remove all Sensors
        removeAllSensors();
        break;
      case 11: // Get Alert Phone #
        getAlertPhone();
        break;
      case 12: // Set Alert Phone #
        setAlertPhone();
        break;
      case 13: // Get Portal Phone #
        getPortalPhone();
        break;
      case 14: // Set Portal Phone #
        setPortalPhone();
        break;
      case 15: // Get Portal Notification Freq.
        getPortalFreq();
        break;
      case 16: // Set Portal Notification Freq.
        setPortalFreq();
        break;
      case 17:
        getLoggingFreq();
        break;
      case 18:
        setLoggingFreq();
        break;
      case 19:
        getHubTime();
        break;
      case 20:
        setHumTime();
        break;
      default:
        BTSerial.println(F("Invalid Command"));
        break;
    }
  }

  // Clean up buffers
  clearAllBuffers();

  // Flush BTSerial TX
  BTSerial.flush();

  // Clean up the rest of BTSerial Rx
  while(BTSerial.available()) 
    BTSerial.read();

  // Clear Busy Status
  digitalWrite(BUSY_LED, LOW);
  
}
