//==============================================================================
// Program:      Hub_B.ino
// Author:       Haowen Zhang
// Target:       Hub B
// Date:         2016/08/10
//==============================================================================


//=====[ INCLUDE ]==============================================================
#include <DS3231.h>
#include <SoftwareSerial.h>
#include <TalkB2A.h>
#include <SPI.h>
#include <SD.h>


//=====[ PINS ]=================================================================
//SD Card
#define CS          10
#define MOSI        11
#define MISO        12
#define CLK         13

#define FLAG        8
#define ALARM       2

//RTC
//      SDA         A4
//      SCL         A5

// SerialB2A
#define DATAIN       7
#define DATAOUT      6


//=====[ CONSTANTS ]============================================================
// Temperature and Humidity definitions
#define criticalTemp 50
#define criticalHum  80


//=====[ VARIABLES ]============================================================

bool Century=false;
bool h12;
bool PM;
char* AlertPhone = "+19546621746";


//=====[ OBJECTS ]==============================================================
TalkB2A SerialB2A(DATAIN,DATAOUT);
DS3231 Clock;

//=====[ SETUP ]================================================================
void setup() {
  Wire.begin();// RTC uses wire libaray
  Serial.begin(9600);
  SerialB2A.begin(9600);
  
  pinMode(ALARM,INPUT);
  pinMode(FLAG,OUTPUT);
  digitalWrite(FLAG,HIGH);
  
  Serial.println(F("Setting up SD card..."));
  if (!SD.begin(CS)) {
    Serial.println(F("SD failed!"));
    return;
  }
  SerialB2A.ClearAllBuffer();
  Serial.println(F("Setup done."));
  digitalWrite(FLAG,LOW);
}
//==============================================================================


//=====[ LOOP ]=================================================================
void loop() {
  //Check if it's time to remind Hub_A to check sensors
  if(digitalRead(ALARM)==HIGH){
    digitalWrite(FLAG, HIGH);
  }
  
  //Standby and wait for commands from Hub_A
  if(SerialB2A.available()>0){
    Serial.println(F("Start Reading"));
    SerialB2A.listen();
    SerialB2A.ReadCommand();
    int i = atoi(SerialB2A.Parameter[0]);
    Serial.flush();
    switch(i){
//      case 0:
//      {SerialB2A.ReplyStatus(Reset());break;}
      case 1:
      {SerialB2A.ReplyStatus(SetupTime());break;}
      case 2:
      {GetTime();break;}
      case 3:
      {SerialB2A.ReplyStatus(SetupDate());break;}
      case 4:
      {GetDate();break;}
//      case 5:
//      {CheckErrorHistory();break;}
      case 6:
      {SerialB2A.ReplyStatus(RecordData());break;}
      case 7:
      {GetData();break;}
      case 8:
      {SerialB2A.ReplyStatus(SetMacAddress());break;}
      case 9:
      {GetMacAddress(atoi(SerialB2A.Parameter[1]));break;}
      case 10:
      {SerialB2A.ReplyStatus(SetPortalPhone());break;}
      case 11:
      {GetPortalPhone();break;}
      case 12:
      {SerialB2A.ReplyStatus(SetNotificationPhone());break;}
      case 13:
      {GetNotificationPhone();break;}
      case 14:
      {SerialB2A.ReplyStatus(SetPortalNotificationFreq());break;}
      case 15:
      {GetPortalNotificationFreq();break;}
      case 16:
      {SerialB2A.ReplyStatus(SetLoggingFreq());break;}
      case 17:
      {GetLoggingFreq();break;}
      case 18:
      {SerialB2A.ReplyStatus(SetCritTemp());break;}
      case 19:
      {GetCritTemp();break;}
      case 20:
      {SerialB2A.ReplyStatus(SetCritHumidity());break;}
      case 21:
      {GetCritHumidity();break;}
      case 22:
      {SerialB2A.ReplyStatus(SetHubID());break;}
      case 23:
      {GetHubID();break;}
      case 24:
      {TurnOffFlag();break;}
      case 25:
      {RemoveAllSensor();break;}
    }

    Serial.println(F("done"));
  }
  else delay(100);

}
