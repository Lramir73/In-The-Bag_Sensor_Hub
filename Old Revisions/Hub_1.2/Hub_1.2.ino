#include <DS3231.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <SerialGSM.h>

/**************** Pin definitions ****************/ 
#define CS          8
#define MOSI        11
#define MISO        12
#define CLK         13

// Bluetooth Pins
#define BT_RX       6
#define BT_TX       7

// GSM Pins
#define GSM_RX      2
#define GSM_TX      3

// Temperature and Humidity definitions
#define criticalTemp 50
#define criticalHum 80

/**************** Variable definitions ****************/ 
SoftwareSerial bluetooth(BT_RX, BT_TX);
SerialGSM cell(GSM_RX, GSM_TX);
DS3231 Clock;

byte tempAvg;
byte humAvg;
byte second, minute, hour, date, month, year, DoW; 

char phone_number[] = "+19546621746";
char message[] = "Check The Bag";

void setup() 
{
  Serial.begin(115200); // Start up for serial monitor
  Wire.begin();         // Start up for RTC
  cell.begin(9600);     // Start up for GSM
  cell.Verbose(true);
  cell.Boot();
  cell.FwdSMS2Serial();

  bluetooth.begin(9600); // Start up for Bluetooth
  pinMode(6, INPUT);

  // Check if SD card is available
  if (!SD.begin(CS))
  {
    Serial.println(F("Card failed"));
    return;
  }
  Serial.println(F("Setup done"));
}

void loop() 
{
  tempAvg = 32;
  humAvg = 65;

  // Get data from the sensors
  getSensorData();

  // Get the current time
  Clock.getTime(year, month, date, DoW, hour, minute, second);

  // Print the time and sensor data to serial monitor
  printTimeStamp();
  printSensorData();

  // Save data into SD card
  recordData();

  // Send SMS alert if needed
  if((tempAvg > criticalTemp) || (humAvg > criticalHum))
  {
    cell.Rcpt(phone_number);
    cell.Message(message);
    cell.SendSMS();
    Serial.println(F("Message Sent"));
    cell.DeleteAllSMS();
  }
}


void getSensorData()
{
  byte i = 16;
  
  // Take 16 readings from the sensor (Might want to turn this into array in flash)
  while (i++ < 16) 
  {
    if (bluetooth.available() > 0)
    {
      byte hum, temp;  
      hum = bluetooth.read();
      delay(100);
      temp = bluetooth.read();
      tempAvg += temp;
      humAvg += hum;
    }
  }
  
  // Get the average of the readings
  humAvg = humAvg / 16;
  tempAvg = tempAvg / 16;
}


void printTimeStamp()
{
  // Print date to serial monitor
  Serial.print(F("20"));
  Serial.print(year, DEC);
  Serial.print('-');
  Serial.print(month, DEC);
  Serial.print('-');
  Serial.print(date, DEC);
  
  // Print Time to serial monitor
  Serial.print(' ');
  Serial.print(hour, DEC);
  Serial.print(':');
  Serial.print(minute, DEC);
  Serial.print(':');
  Serial.print(second, DEC);
  Serial.print('\n');
}


void printSensorData()
{
  // Display the reading on the monitor
  Serial.print(F("\nHumidity: "));
  Serial.println(humAvg);
  Serial.print(F("\nTemperature: "));
  Serial.println(tempAvg);
}


void recordData()
{
  // Open a file and store the readings on the file
  File dataFile = SD.open(F("DATALOG.TXT"), FILE_WRITE);

  if (dataFile)
  {
    // Write sensor data to file
    dataFile.print(F("\nHumidity: "));
    dataFile.println(humAvg);
    dataFile.print(F("\nTemperature: "));
    dataFile.println(tempAvg);
    
    // Write data to file
    dataFile.print(F("20"));
    dataFile.print(year,DEC);
    dataFile.print('-');
    dataFile.print(month,DEC);
    dataFile.print('-');
    dataFile.print(date,DEC);

    // Write time to file
    dataFile.print(' ');
    dataFile.print(hour,DEC);
    dataFile.print(':');
    dataFile.print(minute,DEC);
    dataFile.print(':');
    dataFile.print(second,DEC);
    dataFile.print('\n');
  
    dataFile.close();
    Serial.println(F("Data Saved.\n"));
  }
  else
  {
    Serial.println(F("Error Saving Data"));
  }
}

