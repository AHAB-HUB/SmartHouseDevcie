/*
   This is the main class to handle the esp8266 board functionalities
   and to implement them in a specific order. These functionalities are such as:
    - Establishing wifi connection
    - Establishing connection with server
    - Handle communication errors
    - Receive data from server and forward it to the arduino board
*/

#include <ESP8266WiFi.h>
#include "RestClient.h"

const char* ssid     = "ASUS_2G";
const char* password = "greatdaisy150";
RestClient rest;
bool newData = false;
String recData;

void setup() {

  Serial.begin(115200); // start serial with the ESP defualt baud rate
  delay(1000);
  Serial.flush();

  establishWifiConnection("Connecting to ");
}

void loop() {

  if ( WiFi.status() == WL_CONNECTED ) {
    receiveData();

    if (recData == "ready") {
      Serial.write(rest.getDevices().c_str());
      Serial.write('\n');
    } else {
      serializeAndPost();
    }
  } else {
    establishWifiConnection("Wifi connection lost! Attempting to reconnect.. ");
  }
}

void receiveData() {
  while (Serial.available() == 0) {}

  recData =  Serial.readStringUntil('\n');
}

void serializeAndPost() {
  Serial.write("recevied from serial: ");
  Serial.write(recData.c_str());
  Serial.write('\n');
}

void establishWifiConnection(String message) {

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    WiFi.begin(ssid, password);
  }
  Serial.print('\n');
  Serial.print(message);
  Serial.print(ssid); Serial.println(" ...");

  for ( int i = 1; WiFi.status() != WL_CONNECTED; i++ ) {
    delay(1000);
    Serial.print(i); Serial.print(' ');
  }

  Serial.print('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
}
