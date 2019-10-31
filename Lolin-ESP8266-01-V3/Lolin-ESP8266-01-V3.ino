/*
   This is the main class to handle the esp8266 board functionalities
   and to implement them in a specific order. These functionalities are such as:
    - Establishing wifi connection
    - Establishing connection with server
    - Handle communication errors
    - Receive data from server and forward it to the arduino board
*/

#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include "RestClient.h"

const char* ssid     = "ASUS_2G";
const char* password = "greatdaisy150";

//TODO connect to wifi,establish connection with the server
void setup() {

  Serial.begin(115200); // Start the Serial communication to send messages to the computer
  Serial.println("AT+IPR=9600");
  delay(1000);
  Serial.end();
  Serial.begin(9600);
  delay(10);
  
  Serial.println('\n');

  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  // end wifi connection
}

//TODO get data from server, check if data is valid and usable, send data to arduino
void loop() {
  
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
   
          RestClient rest;
          Serial.println(rest.getDevices());
          Serial.flush();  
    }
    
    delay(2000);    //Send a request every 30 seconds
    }

  
