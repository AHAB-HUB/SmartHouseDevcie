#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "Device.h"

using namespace std;
/*
 * This is the main class to handle the arduino board functionalities
 * and to implement them in a specific order. These functionalities are as such as:
 * - Retriving data from the esp8266 board
 * - translate this data and converte it into states
 * - Apply these states into the connected devices with arduino
 */

  SoftwareSerial ESPserial (0, 1);


//TODO 
void setup() {
  Serial.begin(9600);
  ESPserial.begin(9600);
  
  while(!Serial){}
  
}

//TODO get data from esp, handle errors, apply the new states 
void loop() {
 
    // listen for communication from the ESP8266 and then write it to the serial monitor
    if ( ESPserial.available() )       {
        String c = ESPserial.readString();
        Serial.print(c);
      }
    
    // listen for user input and send it to the ESP8266
    // if ( Serial.available() )       {  ESPserial.write( Serial.read() );  }


    Device device(12, "wifi", "OFF");
    Device devices[15];
    devices[0] = device;
    
    Serial.println( devices[0].getStatus() );
    Serial.println(devices[0].getName());
}
