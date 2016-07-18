//==============================================================================
// Program:      Hub_1.4.ino
// Author:       
// Version:      1.4
// Target:       UNO R3, IDE 1.6.9 
// Date:         2016/07/11
// Notes:        
//               Uses SoftwareSerial I/O, Linked Lists
// Reference:    
//==============================================================================

//=====[ INCLUDE ]==============================================================
#include <HM_10.h>
#include <LinkedList.h>

//=====[ PINS ]=================================================================
#define RX  2
#define TX  3
#define KEY  4
#define STATE  5

//=====[ CONSTANTS ]============================================================
#define MAX_ADDRESSES 6
#define BUFF_SIZE 20
#define CMD_SIZE 3
#define DATA_SIZE 16

#define HUB_ID_SIZE 4
#define MAX_SENSORS 5
#define NAME_SIZE 12
#define ADDR_SIZE 12


//=====[ CLASSES ]==============================================================
class Sensor
{
  public:
    char name[NAME_SIZE + 1];
    char address[ADDR_SIZE + 1];
};

//=====[ VARIABLES ]============================================================
char Buffer[BUFF_SIZE];  // Serial buffer
char Command[CMD_SIZE];  // Arbitrary Value for command size
char Data[DATA_SIZE];    // Arbitrary Value for data size

char HubID[HUB_ID_SIZE];
char SensorList[MAX_SENSORS][ADDR_SIZE+1];


HM_10 BTSerial(RX, TX, KEY, STATE);

//=====[ SUBROUTINES ]==========================================================
void BluetoothParser(void)
{
  byte ByteCount =  BTSerial.readBytesUntil(';', Buffer, BUFF_SIZE-1);
  
  if(ByteCount  > 0) 
  {
    // Print out entire buffer
    BTSerial.print("Buffer  : ");
    BTSerial.println(Buffer);

    // Parse Command and Data parameters
    strcpy(Command,strtok(Buffer," "));             
    strcpy(Data,strtok(NULL,";"));

    // Protect from overflowing buffers
    Command[CMD_SIZE-1] = '\0';
    Data[DATA_SIZE-1] = '\0';

    // Print out Command
    BTSerial.print("Command : ");
    BTSerial.println(Command);

    // Print out Data parameters
    BTSerial.print("Data    : ");
    BTSerial.println(Data);

    // Convert command into a byte
    byte cmd = atoi(Command);
    
    // Execute Command
    switch(cmd)
    {
      case 1: // Factory Reset (Maybe)
        
        break;
      case 2: // Get MAC address (May not be needed)
        
        break;
      case 3: // Get Hub Name (May not be needed)

        break;
      case 5: // Get Hub ID
        getHubID();
        break;
      case 6: // Set Hub ID
        setHubID();
        break;
      case 7: // Get List of Sensors
        getSensorList();
        break;
      case 8: // Add a Sensor
        
        break;
      case 9: // Remove a Sensor
        
        break;
      case 10: // Remove all Sensor
        
        break;
      case 11: // Get Notification Phone #
      
        break;
      case 12: // Set Notification Phone #
      
        break;
      case 13: // Get Portal Notification Freq.
      
        break;
      case 14: // Set Portal Notification Freq.
      
        break;
      case 15: // Get Logging Frequency
      
        break;
      case 16: // Set Logging Frequency
      
        break;
      case 17: // Get Hub Time
      
        break;
      case 18: // Set Hub Time
      
        break;
      case 19: // Get Hub Date
      
        break;
      case 20: // Set Hub Date
      
        break;
      case 21: // Get Critical Temperature
      
        break;
      case 22: // Set Critical Temperature
      
        break;
      case 23: // Get Critical Humidity
      
        break;
      case 24: // Set Critical Humidity
      
        break;
      default:
        BTSerial.println("Invalid Command");
        break;
      
    }
  }
  
  clearBuffers();
  BTSerial.flush();
  while(BTSerial.available()) 
    BTSerial.read();
}

void clearBuffers()
{
  memset(Buffer, 0, BUFF_SIZE);   // Clear contents of Buffer
  memset(Command, 0, CMD_SIZE);   // Clear contents of Buffer
  memset(Data, 0, DATA_SIZE);     // Clear contents of Buffer
}

void getHubID()
{
  BTSerial.print(HubID);
}

void setHubID()
{
  memcpy(HubID, Data, HUB_ID_SIZE);
}

void getSensorList()
{
  for(byte i = 0; i < MAX_SENSORS; i++)
  {
    BTSerial.print(i+1, DEC);
    BTSerial.print(": ");
    BTSerial.println(SensorList[i]);
  }
}

byte findOpenSensor()
{
  for(byte i = 0; i < MAX_SENSORS; i++)
  {
    if(SensorList[i][0] == '\0')
      return i;
  }
  return MAX_SENSORS;
}

void addSensor()
{
  byte i = findOpenSensor();
  if(i != MAX_SENSORS)
  {
    memcpy(SensorList[i], Data, ADDR_SIZE);
    SensorList[i][ADDR_SIZE] = '\0';
  }
  else
    BTSerial.println(F("Too Many Sensors!"));
}

//=====[ SETUP ]================================================================
void setup() 
{
  Serial.begin(9600);      // For the Arduino IDE Serial Monitor
  BTSerial.begin(9600);    // HM-10 default speed 

  memset(HubID, '0', HUB_ID_SIZE);
  memset(SensorList, '\0', MAX_SENSORS * (ADDR_SIZE+1));

  Serial.println(F("\nSetup Done!\n"));
}
//==============================================================================


//=====[ LOOP ]=================================================================
void loop() 
{
  if(BTSerial.available())
    BluetoothParser();

}
//==============================================================================

