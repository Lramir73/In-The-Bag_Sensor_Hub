/***** HUB_B SETUP COMMAND *****/
void sendSetupCommand(char * cmd, char * dest)
{
  // Send a command to Hub_B
  Serial.print(F("Sent: "));
  Serial.println(cmd);
  SerialB.println(cmd);
  SerialB.flush();

  delay(500);
  
  // Read Response from Hub_B
  clearBuffer();
  byte bytesRead = SerialB.readBytesUntil('\n', Buffer, BUFF_SIZE-1);
  if(bytesRead > 0)
  {
    Serial.print(F("Received: "));
    Serial.println(Buffer);
    strcpy(dest, Buffer);
  }
  
}

void sendCommand(char * cmd)//might just want to change to byte
{
  // Send a command to Hub_B
  Serial.print(F("Sent: "));
  Serial.print(cmd);
  Serial.print(' ');
  Serial.println(Data);
  Serial.flush();
  
  SerialB.print(cmd);
  SerialB.print(' ');
  SerialB.println(Data);
  SerialB.flush();
  SerialB.listen();
  
  delay(1000);
  
  // Read Response from Hub_B
  byte bytesRead = SerialB.readBytesUntil('\n', Buffer, BUFF_SIZE-1);
  if(bytesRead > 0)
  {
    Serial.print(F("Received: "));
    Serial.println(Buffer);
  }
  
  BTSerial.listen();
}

void sendGetCommand(byte cmd)
{
  Serial.print(F("Sent: "));
  Serial.println(cmd);
  
  SerialB.listen();
  SerialB.println(cmd);
  SerialB.flush();

  // Read Response from Hub_B
  clearBuffer();
  byte bytesRead = SerialB.readBytesUntil('\n', Buffer, BUFF_SIZE-1);
  if(bytesRead > 0)
  {
    Serial.print(F("Received: "));
    Serial.println(Buffer);
  }
  BTSerial.listen();
}


void sendData(byte num, float temp, float humid)
{
  Serial.println(F("Recording Data..."));
  Serial.print(F("6 "));
  Serial.print(num);
  Serial.print(" ");
  Serial.print(temp);
  Serial.print(" ");
  Serial.println(humid);
  Serial.flush();
  
  SerialB.print("6 ");
  SerialB.print(num);
  SerialB.print(' ');
  SerialB.print(temp);
  SerialB.print(' ');
  SerialB.println(humid);
  SerialB.flush();
  SerialB.listen();

  delay(2000);
  
  // Read Response from Hub_B
  byte bytesRead = SerialB.readBytesUntil('\n', Buffer, BUFF_SIZE-1);
  if(bytesRead > 0)
  {
    Serial.println(Buffer);
    
    if(Buffer[0] == 'd')
      Serial.println(F("Success!"));
  }
  else
    Serial.println(F("Fail!"));
  
  BTSerial.listen();
}

void checkErrorCode()
{
  if(Buffer[0] == 'e')
  {
    Serial.println(F("Error Encountered!"));
    Serial.print(F("Error Code: "));
    Serial.println(Buffer);
  }
}













