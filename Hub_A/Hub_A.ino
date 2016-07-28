//==============================================================================
// Program:      Hub_A.ino
// Author:       
// Version:      1.7
// Target:       Hub A
// Date:         2016/07/28
//==============================================================================

//=====[ INCLUDE ]==============================================================
#include <LinkedList.h>
#include <SerialGSM.h>
#include <HM_10.h>


//=====[ PINS ]=================================================================
// HM-10 Pins
#define RX_BT  2
#define TX_BT  3
#define KEY  4
#define STATE  5

// Hub-B Pins
#define RX_B 6
#define TX_B 7
#define FLAG 11

// Switch Pin
#define MODE_SWITCH 8

// GSM Pins
#define RX_GSM 9
#define TX_GSM 10

// LED Pins
#define MODE_LED 12
#define BUSY_LED 13


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


//=====[ VARIABLES ]============================================================
char Buffer[BUFF_SIZE];        // Serial buffer
char Command[CMD_SIZE];       // Arbitrary Value for command size
char Data[DATA_SIZE];         // Arbitrary Value for data size

char HubID[HUB_ID_SIZE];
char AlertPhone[PHONE_SIZE];      // Notification phone number
char PortalPhone[PHONE_SIZE];     // Portal phone number

byte PortalFreq = 0;          // Portal notification frequency (default = 0)
byte LoggingFreq = 0;         // Sensor logging frequency (default = 0)

bool HubMode = false;
bool flagCheck = false;

int critTemp = 30;
int critHum = 50;


//=====[ CLASSES ]==============================================================
class Sensor
{
  public:
    char address[ADDR_SIZE + 1];
};


//=====[ OBJECTS ]==============================================================
LinkedList<Sensor*> SensorList =  LinkedList<Sensor*>();
HM_10 BTSerial(RX_BT, TX_BT, KEY, STATE);
SoftwareSerial SerialB(RX_B, TX_B);
SerialGSM cell(RX_GSM, TX_GSM);



//=====[ SETUP ]================================================================
void setup() 
{
  Serial.begin(9600);      // For the Arduino IDE Serial Monitor
  SerialB.begin(9600);     // For communication between Atmegas
  BTSerial.begin(9600);    // HM-10 default speed 
//  cell.begin(9600);        // Start up GSM module
  
  
  // Set Pin Modes
  pinMode(FLAG, INPUT);
  pinMode(MODE_SWITCH, INPUT);
  pinMode(MODE_LED, OUTPUT);
  pinMode(BUSY_LED, OUTPUT);

  // Start General Setup Process
  Serial.println(F("Setup Start!"));
  
  // Show that Hub is Busy
  digitalWrite(BUSY_LED, HIGH);

  // Send Initial GSM Functions
//  cell.listen();
//  cell.Verbose(true);
//  cell.Boot();
//  cell.FwdSMS2Serial();
  
  // Clear out buffers
  clearAllBuffers();
  memset(HubID, '0', HUB_ID_SIZE);
  memset(AlertPhone, '0', PHONE_SIZE);
  memset(PortalPhone, '0', PHONE_SIZE);

  // Listen to Bluetooth Serial
  BTSerial.listen();
  
  // Make sure Bluetooth is initially disconnected
  BTSerial.atTEST();
  
  // Read Mode switch and set the mode
  HubMode = digitalRead(MODE_SWITCH);
  if(HubMode)
  {
    BTSerial.atROLE('1');
    Serial.println(F("Entering Hub Mode"));
  }
  else
  {
    BTSerial.atROLE('0');
    Serial.println(F("Entering Config Mode"));
  }

  BTSerial.atRESET();
  digitalWrite(MODE_LED, HubMode);

  // Wait for B Flag to go LOW
  Serial.print(F("Waiting for Hub_B..."));
  while(digitalRead(FLAG) == HIGH);

  Serial.println(F("Hub_B is Ready!"));

  // Listen to Hub_B Serial
  SerialB.listen();

  /***** Get data from SD card *****/ 
  Serial.println(F("Getting Data From SD Card")); 
  // Get Hub ID from Hub_B
  sendSetupCommand("23", HubID);
  
  // Get Alert Phone Number from Hub_B
  sendSetupCommand("13", AlertPhone);
  
  // Get Portal Phone Number from Hub_B
  sendSetupCommand("11", PortalPhone);

  // Clean up Buffers again...
  clearAllBuffers();
  
  // Get Critical Temperature from Hub_B
  sendSetupCommand("19", Data);
  critTemp = atoi(Data);
  memset(Data, '0', DATA_SIZE);
  
  // Get Critical Humidity from Hub_B
  sendSetupCommand("21", Data);
  critTemp = atoi(Data);
  memset(Data, '0', DATA_SIZE);
  
  // Get Sensor MAC Addresses from Hub_B
  sendSetupCommand("9 0", Data);

  // Add the Sensor to SensorList
  Sensor * newSensor = new Sensor();
  memcpy(newSensor->address, Data, ADDR_SIZE);
  newSensor->address[ADDR_SIZE] = '\0';

  SensorList.add(newSensor);
  Serial.print("Sensor 0: ");
  Serial.println(newSensor->address);

  // Return to listening to Bluetooth Serial
  BTSerial.listen();

  // Clean buffers one last time
  clearAllBuffers();
  
  // Clear Busy LED
  digitalWrite(BUSY_LED, LOW);
  Serial.println(F("Setup Done!"));
}
//==============================================================================


//=====[ LOOP ]=================================================================
void loop() 
{
  if(digitalRead(MODE_SWITCH) != HubMode)
  {
    HubMode = !HubMode; // Invert the HubMode
    BTSerial.atTEST();  // Make sure the module is disconnected
    
    // Switch to the correct role
    if(HubMode)
    {
      BTSerial.atROLE('1'); // Set to Master
      Serial.println(F("Entering Hub Mode"));
    }
    else
    {
      BTSerial.atROLE('0'); // Set to Slave
      Serial.println(F("Entering Config Mode"));
    }
    
    // Reset Bluetooth Module
    BTSerial.atRESET();

    // Set the Mode LED
    digitalWrite(MODE_LED, HubMode);
  }

  
  if(HubMode)
  {
    // Bluetooth is in Master Role (Used for Sensor Checks)
    
    // Wait for Alarm Flag
    Serial.println(F("Waiting for Alarm..."));
    while(digitalRead(FLAG) == LOW);

    // Check the first Sensor
    checkSensor(0);
    
    // Wait until mode is switched back
    Serial.println(F("Waiting To Switch Back To Config Mode..."));
    while(HubMode == digitalRead(MODE_SWITCH));
  }
  else
  {
    // Bluetooth is in Slave Role (Used for being configured by the App)
    if(BTSerial.available())
      BluetoothParser();
  }
  

}
//==============================================================================
void clearAllBuffers()
{
  memset(Buffer, 0, BUFF_SIZE);   // Clear contents of Buffer
  memset(Command, 0, CMD_SIZE);   // Clear contents of Buffer
  memset(Data, 0, DATA_SIZE);     // Clear contents of Buffer
}

void clearBuffer()
{
  memset(Buffer, 0, BUFF_SIZE);   // Clear contents of main Buffer
}

void printBuffers()
{
  // Print out Command
  BTSerial.print("Command : ");
  BTSerial.println(Command);

  // Print out Data parameters
  BTSerial.print("Data    : ");
  BTSerial.println(Data);
}


//==============================================================================
void checkSensor(byte num)
{
  // Get Sensor MAC Address from SensorList
  Serial.println("Starting Sensor Check!");
  Sensor * s = SensorList.get(0);

  // Connect to the Sensor
  Serial.println("Connecting to Sensor!");  
  BTSerial.atCO('N',s->address);
  delay(1000);

  Serial.println("Requesting Data from Sensor...");
  float t = 0;
  float h = 0;

  if(BTSerial.connected())
  {
    // Get Temperature from the sensor
    BTSerial.println("1");
    delay(1000);
    if(BTSerial.available())
    {
      t = BTSerial.parseFloat();
      Serial.print("Temperature: ");
      Serial.print(t);          // Prints Temperature as Float
//      Serial.print(176, BYTE);  // Prints Degree Symbol
      Serial.println(" C");      // Prints Celsius
    }

    // Wait before Sending Another Command
    delay(3000);
  
    // Get Humidity from the sensor
    BTSerial.println("2");
    delay(1000);
    if(BTSerial.available())
    {
      h = BTSerial.parseFloat();
      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.println(" %RH");
    }

    

  }
  // Send Data To Be Recorded In SD Card
  SerialB.listen();
  clearAllBuffers();
  //sendData(0, t, h);



  // Check if critical Temperature or Humidity
  if(t >= critTemp && h >= critHum)
    sendCommand("25 1", Data);
  else if(t >= critTemp)
    sendCommand("25 2", Data);
  else if(h >= critHum)
    sendCommand("25 3", Data);

  Serial.println(F("Sensor Check Done!"));
  
  // Return to listening to Bluetooth
  BTSerial.listen();
  

}







