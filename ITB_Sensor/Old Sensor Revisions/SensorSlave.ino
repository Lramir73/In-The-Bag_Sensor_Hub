/*
serialSlavefinal.ino - main code for the Sensor Circuit. 

Author:         Siddhesh Bhandarkar
Version:        1
Creation Date:  Wed April 13 02:35 am 2016
Filename:       serialSlaveFinal.ino

*/
 
#include <dht11.h>
#include <SoftwareSerial.h>
#include "LowPower.h"
  
SoftwareSerial mySerial(7,6); 
dht11 DHT11; 
  
void setup()  
{
 
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(6, OUTPUT);  
  pinMode(3,OUTPUT); 
}
 
void loop()
{
  int i=0;
  int time_quantum = 0;  //Change based on how long board needs to sleep.

  // Put the Board to sleep.

  Serial.println("Sleeping...");
  digitalWrite(3, LOW);  // LED turn off.
  delay(100);
  while(time_quantum < 4)
  {
     LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
     Time_quantum++;
  }
  delay(100);
  Serial.println("Awake!");
  digitalWrite(3, HIGH);  // LED turn on. 
  delay(8000);


  // Take 16 readings from the sensor and send it to the hub via Bluetooth. 
  while(i<16)
  {
     delay(500);
     int chk = DHT11.read(2);
     int humidity = DHT11.humidity;
     int temp = DHT11.temperature;
     Serial.println(humidity);
     Serial.println(temp); 
     mySerial.write(humidity);
     mySerial.write(temp);
     i++;
  }
}
