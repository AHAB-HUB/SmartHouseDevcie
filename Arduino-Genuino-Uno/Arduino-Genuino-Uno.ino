#include <ArduinoJson.h>
#include "Device.h"

int pin1 = 12;
int pin2 = 13;
int pin3 = 11;
int pin4 = 8;
/*
   This is the main class to handle the arduino board functionalities
   and to implement them in a specific order. These functionalities are as such as:
   - Retriving data from the esp8266 board
   - translate this data and converte it into states
   - Apply these states into the connected devices with arduino
*/

//TODO
void setup() {
  Serial.begin(9600);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);

}

//TODO get data from esp, handle errors, apply the new states
void loop() {

  // listen for communication from the ESP8266 and then write it to the serial monitor
  if ( Serial.available() ) {

    String c = Serial.readStringUntil('\n');
    Serial.println( Serial.available() );
    char * json = const_cast<char*>(c.c_str());
    Serial.println(json);

    readJSON(json);

    //Serial.print(c);
    //    Serial.flush();
  }

  //    Device device(12, "wifi", "OFF");
  //    Device devices[15];
  //    devices[0] = device;
  //
  //Serial.println( devices[0].getStatus() );
  // Serial.println(devices[0].getName());
}


void readJSON( char* json ) {
  const size_t capacity = JSON_ARRAY_SIZE(9) + 9 * JSON_OBJECT_SIZE(3) + 490;
  DynamicJsonDocument doc(capacity);

  DeserializationError err = deserializeJson(doc, json);

 

  JsonObject root_0 = doc[0];
  const char* root_0_deviceId = root_0["deviceId"]; // "1"
  const char* root_0_deviceName = root_0["deviceName"]; // "indoor lamp"
  const char* root_0_deviceStatus = root_0["deviceStatus"]; // "off"

//  Serial.println(root_0_deviceId);
  //Serial.println(root_0_deviceName);
 // Serial.println(root_0_deviceStatus);

  if (strcmp ("off", root_0_deviceStatus) == 0) {
    pinOutput(1, 0, 1, 0);
    Serial.println("The lamp is off");
  } else if (strcmp ("on", root_0_deviceStatus) == 0) {
    pinOutput(0, 0, 1, 0);
    Serial.println("The lamp is on");
  }
}

void pinOutput(int a, int b, int c, int d) {
  digitalWrite(pin1, a);
  digitalWrite(pin2, b);
  digitalWrite(pin3, c);
  digitalWrite(pin4, d);
}
