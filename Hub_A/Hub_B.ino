/***** HUB_B SETUP COMMAND *****/
void sendSetupCommand(char * cmd, char * dest)
{
  // Send a command to Hub_B
  Serial.println(cmd);
  SerialB.println(cmd);
  SerialB.flush();

  delay(500);
  
  // Read Response from Hub_B
  byte bytesRead = SerialB.readBytesUntil('\n', Buffer, BUFF_SIZE-1);
  if(bytesRead > 0)
  {
    Serial.println(Buffer);
    strcpy(dest, Buffer);
  }
  
}

void sendCommand(char * cmd, char * dest)
{
  // Send a command to Hub_B
  Serial.println(cmd);
  SerialB.println(cmd);
  SerialB.flush();

  delay(500);
  
  // Read Response from Hub_B
  byte bytesRead = SerialB.readBytesUntil('\n', Buffer, BUFF_SIZE-1);
  if(bytesRead > 0)
  {
    Serial.println(Buffer);
    strcpy(dest, Buffer);
  }
  
}


void sendCommand(char * cmd)
{
  // Send a command to Hub_B
  Serial.print(cmd);
  Serial.print(' ');
  Serial.println(Data);
  Serial.flush();
  
  SerialB.print(cmd);
  SerialB.print(' ');
  SerialB.println(Data);
  SerialB.flush();

  delay(500);
  
  // Read Response from Hub_B
  byte bytesRead = SerialB.readBytesUntil('\n', Buffer, BUFF_SIZE-1);
  if(bytesRead > 0)
    Serial.println(Buffer);

  
}


void sendData(byte num, float temp, float humid)
{
  Serial.println("Recording Data...");
  SerialB.print("6 ");
  SerialB.print(num);
  SerialB.print(" ");
  SerialB.print(temp);
  SerialB.print(" ");
  SerialB.println(humid);
  SerialB.flush();

  delay(500);
  
  // Read Response from Hub_B
  byte bytesRead = SerialB.readBytesUntil('\n', Buffer, BUFF_SIZE-1);
  if(bytesRead > 0)
  {
    Serial.println(Buffer);
    
    if(Buffer[0] == 'd')
      Serial.println("Success!");
  }
}















