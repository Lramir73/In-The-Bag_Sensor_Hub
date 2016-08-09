void checkSensor(byte num)
{
  // Get Sensor MAC Address from SensorList
  Serial.println(F("Starting Sensor Check!"));
  Sensor * s = SensorList.get(num);

  // Connect to the Sensor
  Serial.println(F("Connecting to Sensor!"));  
  BTSerial.atCO('N',s->address);
  delay(1000);

  Serial.println(F("Requesting Data from Sensor..."));
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
      Serial.print(F("Temperature: "));
      Serial.print(t);          // Prints Temperature as Float
      Serial.write(176);        // Prints Degree Symbol
      Serial.println(F("C"));   // Prints Celsius
    }

    // Wait before Sending Another Command
    delay(2000);
  
    // Get Humidity from the sensor
    BTSerial.println("2");
    delay(1000);
    if(BTSerial.available())
    {
      h = BTSerial.parseFloat();
      Serial.print(F("Humidity: "));
      Serial.print(h);
      Serial.println(F(" %RH"));
    }

    

  }
  
  // Send Data To Be Recorded In SD Card
  SerialB.listen();
  clearAllBuffers();
  sendData(num, t, h);


  // Check if critical Temperature or Humidity
  if(t >= critTemp && h >= critHum)
    sendAlert(1);
  else if(t >= critTemp)
    sendAlert(2);
  else if(h >= critHum)
    sendAlert(3);

  Serial.println(F("Sensor Check Done!"));
  
  // Return to listening to Bluetooth
  BTSerial.listen();
}
