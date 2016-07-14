/*serialMasterfinal.ino - main code for the Sensor Hub.

  Author:         Siddhesh Bhandarkar
  Version:        1
  Creation Date:  Wed April 13 02:35 am 2016
  Filename:       serialMasterFinal.ino

*/
#include <DS3231.h>
#include<SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <SerialGSM.h>
#define criticalTemp 50
#define criticalHum 80
#define chipSelect 8

char cellphone_number[] = "+19546621746";
char message[] = "Check The Bag";

DS3231 Clock;
bool Century = false;
bool h12;
bool PM;

byte i = 16;
byte tempAvg;
byte humAvg;
byte second, minute, hour, date, month, year, DoW; 

SoftwareSerial mySerial(6, 7);
SerialGSM cell(2,3); // RX, TX pins motorola


void setup()  
{
  //Setup RealTime Clock
  Wire.begin();
  Clock.setSecond(50);//Set the second 
  Clock.setMinute(59);//Set the minute 
  Clock.setHour(23);  //Set the hour 
  //Clock.setDoW(5);    //Set the day of the week
  Clock.setDate(31);  //Set the date of the month
  Clock.setMonth(5);  //Set the month of the year
  Clock.setYear(13);  //Set the year (Last two digits of the year)
  
  delay(3000);
  Serial.begin(115200);

  //Setup GSM Module
  cell.begin(9600);
  cell.Verbose(true);
  cell.Boot();
  cell.FwdSMS2Serial();

  //Setup SD Card Module
  mySerial.begin(9600);
  pinMode(6, INPUT);
  if (!SD.begin(chipSelect))
  {
    //Serial.println("Card failed");
    return;
  }
  Serial.println(F("Setup done"));
}

void loop()
{
  tempAvg = 32;
  humAvg = 65;
  
  // Take 16 readings from the sensor
   while (i < 16) 
  {
    if (mySerial.available() > 0)
    {
      byte humidity, temp;
      humidity = mySerial.read();
      delay(100);
      temp = mySerial.read();
      tempAvg += temp;
      humAvg += humidity;
      i++;
    }
  }

  // Get the average of the readings
  humAvg = humAvg / 16;
  tempAvg = tempAvg / 16;

  // Get the current time
  Clock.getTime(year, month, date, DoW, hour, minute, second);
  
  // Display the reading on the monitor
  delay(10000);
  Serial.println(F("\n Humidity: "));
  Serial.println(humAvg);
  Serial.println(F("\n Temperature: "));
  Serial.println(tempAvg);
  Serial.print(F("20"));
  Serial.print(year,DEC);
  Serial.print('-');
  Serial.print(month,DEC);
  Serial.print('-');
  Serial.print(date,DEC);
  Serial.print(' ');
  Serial.print(hour,DEC);
  Serial.print(':');
  Serial.print(minute,DEC);
  Serial.print(':');
  Serial.print(second,DEC);
  Serial.print('\n');

  // Open a file and store the readings on the file
  File dataFile = SD.open("DATALOG.TXT", FILE_WRITE);

  if (dataFile)
  {
    dataFile.println(F("\nHumidity: \n"));
    dataFile.println(humAvg);
    dataFile.println(F("\nTemperature: \n"));
    dataFile.println(tempAvg);
    
    dataFile.print(F("20"));
    dataFile.print(year,DEC);
    dataFile.print('-');
    dataFile.print(month,DEC);
    dataFile.print('-');
    dataFile.print(date,DEC);
    dataFile.print(' ');
    dataFile.print(hour,DEC);
    dataFile.print(':');
    dataFile.print(minute,DEC);
    dataFile.print(':');
    dataFile.print(second,DEC);
    dataFile.print('\n');
  
    dataFile.close();
    Serial.println(F("File is closed.\n"));
  }
  else
  {
    Serial.println(F("error"));
  }


  // Send message to the farmer if required

  //if ((tempAvg > criticalTemp) || (humAvg > criticalHum))
  if(0)
  {
    cell.Rcpt(cellphone_number);
    cell.Message(message);
    cell.SendSMS();
    Serial.println(F("Message Sent"));
    cell.DeleteAllSMS();
  }
}






