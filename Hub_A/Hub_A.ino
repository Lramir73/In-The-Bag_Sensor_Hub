//==============================================================================
// Program:      Hub_1.4.ino
// Author:       
// Version:      1.5
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
// HM-10 Pins
#define RX  2
#define TX  3
#define KEY  4
#define STATE  5

// MCU-B Pins
#define RX_B 6
#define TX_B 7
#define B_FLAG 8

// GSM Pins
#define RX_GSM 9
#define TX_GSM 10

// LED Pins
#define MODE_LED 11
#define BUSY_LED 12

// Switch Pin
#define MODE_SWITCH 13

//=====[ CONSTANTS ]============================================================
#define MAX_ADDRESSES 6
#define BUFF_SIZE 20
#define CMD_SIZE 3
#define DATA_SIZE 16

#define HUB_ID_SIZE 4
#define PHONE_SIZE 15
#define MAX_SENSORS 5
#define NAME_SIZE 12
#define ADDR_SIZE 12

#define MIN_CMD 0
#define MAX_CMD 26

//=====[ CLASSES ]==============================================================
class Sensor
{
  public:
    char address[ADDR_SIZE + 1];
};

//=====[ VARIABLES ]============================================================
char Buffer[BUFF_SIZE];  // Serial buffer
char Command[CMD_SIZE];  // Arbitrary Value for command size
char Data[DATA_SIZE];    // Arbitrary Value for data size

char HubID[HUB_ID_SIZE];
char AlertPhone[PHONE_SIZE];  // Notification phone number
char PortalPhone[PHONE_SIZE]; // Portal phone number

HM_10 BTSerial(RX, TX, KEY, STATE);
SoftwareSerial SerialB(RX_B, TX_B);
LinkedList<Sensor*> SensorList =  LinkedList<Sensor*>();



//=====[ SETUP ]================================================================
void setup() 
{
  Serial.begin(9600);      // For the Arduino IDE Serial Monitor
  SerialB.begin(9600);     // For communicatiob between Atmegas
  BTSerial.begin(9600);    // HM-10 default speed 

  // Set Pin Modes
  pinMode(B_FLAG, INPUT);
  pinMode(MODE_SWITCH, INPUT);
  pinMode(MODE_LED, OUTPUT);
  pinMode(BUSY_LED, OUTPUT);

  // Show that Hub is Busy
  digitalWrite(BUSY_LED, HIGH);
  
  // Clear out buffers
  clearBuffers();
  memset(HubID, '0', HUB_ID_SIZE);
  memset(AlertPhone, '0', PHONE_SIZE);
  memset(PortalPhone, '0', PHONE_SIZE);

  // Read Mode switch and set the mode
  
  
  // Wait for B Flag to go LOW
  while(digitalRead(B_FLAG) != LOW);

  // Get data from SD card
  
  
  // Show Hub is no Busy
  digitalWrite(BUSY_LED, LOW);
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

//=====[ SUBROUTINES ]==========================================================

/***** BLUETOOTH PARSER *****/
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

    // Print out command and data buffers
    printBuffers();

    // Convert command into a byte
    byte cmd = atoi(Command);

    // Check if Busy status needs to be set
    if(cmd > MIN_CMD && cmd < MAX_CMD)
      digitalWrite(BUSY_LED, HIGH);
      
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
        getAlertPhone();
        break;
      case 14: // Set Portal Phone #
        setAlertPhone();
        break;
      case 15: // Get Portal Notification Freq.
      
        break;
      case 16: // Set Portal Notification Freq.
      
        break;
      case 17: // Get Logging Frequency
      
        break;
      case 18: // Set Logging Frequency
      
        break;
      case 19: // Get Hub Time
      
        break;
      case 20: // Set Hub Time
      
        break;
      case 21: // Get Hub Date
      
        break;
      case 22: // Set Hub Date
      
        break;
      case 23: // Get Critical Temperature
      
        break;
      case 24: // Set Critical Temperature
      
        break;
      case 25: // Get Critical Humidity
      
        break;
      case 26: // Set Critical Humidity
      
        break;
      default:
        BTSerial.println("Invalid Command");
        break;
      
    }
  }

  // Clean up buffers
  clearBuffers();

  // Flush BTSerial TX
  BTSerial.flush();

  // Clean up the rest of BTSerial Queue
  while(BTSerial.available()) 
    BTSerial.read();

  // Clear Busy Status
  digitalWrite(BUSY_LED, LOW);
  
}

/***** PRINT BUFFERS *****/
void printBuffers()
{
  // Print out Command
  BTSerial.print("Command : ");
  BTSerial.println(Command);

  // Print out Data parameters
  BTSerial.print("Data    : ");
  BTSerial.println(Data);
}

/***** BUFFER CLEANER *****/
void clearBuffers()
{
  memset(Buffer, 0, BUFF_SIZE);   // Clear contents of Buffer
  memset(Command, 0, CMD_SIZE);   // Clear contents of Buffer
  memset(Data, 0, DATA_SIZE);     // Clear contents of Buffer
}

/***** CASE 5 *****/
void getHubID()
{
  BTSerial.print(HubID);
}

/***** CASE 6 *****/
void setHubID()
{
  memcpy(HubID, Data, HUB_ID_SIZE);
  // Send TBD Command to Hub_B
}

/***** CASE 7 *****/
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

/***** CASE 8 *****/
void addSensor()
{
  Sensor * newSensor = new Sensor();
  memcpy(newSensor->address, Data, ADDR_SIZE);
  newSensor->address[ADDR_SIZE] = '\0';

  SensorList.add(newSensor);
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
  memcpy(AlertPhone, Data, PHONE_SIZE);
  // Send Set Command to Hub_B
}

/***** CASE 13 *****/

//==============================================================================
