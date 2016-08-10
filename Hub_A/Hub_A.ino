//==============================================================================
// Program:      Hub_A.ino
// Author:       Luis Ramirez
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
char Command[CMD_SIZE];        // Arbitrary Value for command size
char Data[DATA_SIZE];          // Arbitrary Value for data size

char HubID[HUB_ID_SIZE];
char AlertPhone[PHONE_SIZE];   // Notification phone number
char PortalPhone[PHONE_SIZE];  // Portal phone number

byte PortalFreq = 0;           // Portal notification frequency (default = 0)
byte LoggingFreq = 0;          // Sensor logging frequency (default = 0)

int critTemp = 0;
int critHum = 0;

bool HubMode = false;

//=====[ CLASSES ]==============================================================
class Sensor
{
  public:
    char address[ADDR_SIZE + 1];
};


//=====[ OBJECTS ]==============================================================
LinkedList<Sensor*> SensorList = LinkedList<Sensor*>();
HM_10 BTSerial(RX_BT, TX_BT, KEY, STATE);
SoftwareSerial SerialB(RX_B, TX_B);
SerialGSM cell(RX_GSM, TX_GSM);


//=====[ SETUP ]================================================================
void setup() 
{
  Serial.begin(9600);      // For the Arduino IDE Serial Monitor
  SerialB.begin(9600);     // For communication between Atmegas
  BTSerial.begin(9600);    // HM-10 default speed 
  cell.begin(9600);        // Start up GSM module
  
  SerialB.setTimeout(2000);
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
  cell.listen();
  cell.Verbose(true);
//  cell.Boot();
  cell.FwdSMS2Serial();
  
  // Clear out buffers
  clearAllBuffers();
  clearSettings();

  // Listen to Bluetooth Serial
  BTSerial.listen();

  // Setup the Bluetooth Mode
  setupBluetooth();

  // Wait for B Flag to go LOW
  Serial.print(F("Waiting for Hub_B..."));
  while(digitalRead(FLAG) == HIGH);
  Serial.println(F("Hub_B is Ready!"));

  // Listen to Hub_B Serial
  SerialB.listen();

  /***** Get data from SD card *****/ 
  setupInitial();
  setupSensors();

  // Return to listening to Bluetooth Serial
  BTSerial.listen();

  // Clean buffers one last time
  clearAllBuffers();

  // Show recieved values
  printSetup();
  
  // Clear Busy LED
  digitalWrite(BUSY_LED, LOW);
  Serial.println(F("Setup Done!"));
}
//==============================================================================


//=====[ LOOP ]=================================================================
void loop() 
{
  // Check if the hub mode needs to be changed
  if(checkModeChange())
    switchMode();

  if(HubMode) // Bluetooth is in Master Role (Used for Sensor Checks)
  {
    // Wait for Alarm Flag or for the Mode Switch
    Serial.println(F("Waiting for Alarm..."));
    while(digitalRead(FLAG) == LOW && !checkModeChange());

    // Check if there was a change in the Mode Switch, if not then check sensors
    if(checkModeChange())
      switchMode();
    else
      checkAllSensors();
  }
  else  // Bluetooth is in Slave Role (Used for being configured by the App)
  {
    if(BTSerial.available())
      BluetoothParser();
  }
  
}
//==============================================================================









