#include <SoftwareSerial.h>
#include <LowPower.h>
#include <DHT.h>

#define DHTTYPE DHT22

#define DHTPIN 2

#define BT_RX 7
#define BT_TX 6

#define BUFF_SIZE 20
#define CMD_SIZE 3
#define DATA_SIZE 16

SoftwareSerial BTSerial(BT_RX, BT_TX); 
DHT dht(DHTPIN, DHTTYPE);

char Buffer[BUFF_SIZE];  // Serial buffer
char Command[CMD_SIZE];  // Arbitrary Value for command size
char Data[DATA_SIZE];    // Arbitrary Value for data size

void setup() 
{
  BTSerial.begin(9600);
  dht.begin();

  // Clear out buffers
  clearBuffers();
  
}

void loop() 
{
  if(BTSerial.available())
    BluetoothParser();
}


void BluetoothParser(void)
{
  byte ByteCount =  BTSerial.readBytesUntil('\n', Buffer, BUFF_SIZE-1);
  
  if(ByteCount  > 0) 
  {
    // Print out entire buffer
//    BTSerial.print("Buffer  : ");
//    BTSerial.println(Buffer);

    // Parse Command and Data parameters
    strcpy(Command,strtok(Buffer," "));             
    strcpy(Data,strtok(NULL,";"));

    // Protect from overflowing buffers
    Command[CMD_SIZE-1] = '\0';
    Data[DATA_SIZE-1] = '\0';

    // Convert command into a byte
    byte cmd = atoi(Command);
      
    // Execute Command
    switch(cmd)
    {
      case 1: // Get Temperature
      {
        float t = dht.readTemperature();
        BTSerial.println(t);
        delay(2000);
        break;
      }
      case 2: // Get Humidity
      {
        float h = dht.readHumidity();
        BTSerial.println(h);
        delay(2000);
        break;
      }
      case 3: // Get Dew Point

      default:
      {
        BTSerial.println("Invalid Command");
        break;
      }
    }
  }

  // Clean up buffers
  clearBuffers();

  // Flush BTSerial TX
  BTSerial.flush();

  // Clean up the rest of BTSerial Queue
  while(BTSerial.available()) 
    BTSerial.read();
  
}

void clearBuffers()
{
  memset(Buffer, 0, BUFF_SIZE);   // Clear contents of Buffer
  memset(Command, 0, CMD_SIZE);   // Clear contents of Buffer
  memset(Data, 0, DATA_SIZE);     // Clear contents of Buffer
}
