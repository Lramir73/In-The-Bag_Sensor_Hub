/***** CASE 1 *****/
int SetupTime(){
  Clock.setHour(atoi(SerialB2A.Parameter[1]));
  Clock.setMinute(atoi(SerialB2A.Parameter[2]));
  Clock.setSecond(atoi(SerialB2A.Parameter[3]));
  SerialB2A.ClearAllBuffer();
  return 0;
}

/***** CASE 2 *****/
void GetTime(){
  char temp[2];
  itoa(Clock.getHour(h12,PM),temp,10);
  SerialB2A.StartGet(temp);
  itoa(Clock.getMinute(),temp,10);
  SerialB2A.AddParameter(temp);
  itoa(Clock.getSecond(),temp,10);
  SerialB2A.AddParameter(temp);
  Serial.println(SerialB2A.Buffer);
  SerialB2A.SendData();
  SerialB2A.ClearAllBuffer();
}

/***** CASE 3 *****/
int SetupDate(){
  Clock.setYear(atoi(SerialB2A.Parameter[1]));
  Clock.setMonth(atoi(SerialB2A.Parameter[2]));
  Clock.setDate(atoi(SerialB2A.Parameter[3]));
  SerialB2A.ClearAllBuffer();
  return 0;
}

/***** CASE 4 *****/
void GetDate(){
  char temp[2];
  SerialB2A.StartGet("20");
  itoa(Clock.getYear(),temp,10);
  strcat(SerialB2A.Buffer,temp);// To make the format as 20xx.
  itoa(Clock.getMonth(Century),temp,10);
  SerialB2A.AddParameter(temp);
  itoa(Clock.getDate(),temp,10);
  SerialB2A.AddParameter(temp);
  Serial.println(SerialB2A.Buffer);
  SerialB2A.SendData();
  SerialB2A.ClearAllBuffer();
}

