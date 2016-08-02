void sendAlert(byte txt)
{ 
  cell.listen();
  delay(500);
  cell.FwdSMS2Serial();
  cell.Rcpt(AlertPhone);
  
  switch(txt)
  {
    case 1:
      cell.Message("Critical Temperature and Humidity!");
      break;
    case 2:
      cell.Message("Critical Temperature!");
      break;
    case 3:
      cell.Message("Critical Humidity!");
      break;
  }
  
  cell.SendSMS();
  cell.DeleteAllSMS();
  
}

void printBytes(char * buff)
{
  for(byte i = 0; i < sizeof(buff); i++)
  {
    Serial.print(buff[i]);
    Serial.print(' ');
  }
  Serial.println();
  
}

