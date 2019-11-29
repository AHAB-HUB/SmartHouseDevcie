#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>

#include <ArduinoJson.h>
#include "Device.h"

/*
   This is the main class to handle the arduino board functionalities
   and to implement them in a specific order. These functionalities are as such as:
   - Retriving data from the esp8266 board
   - translate this data and converte it into states
   - Apply these states into the connected devices with arduino
*/
int pin1 = 12;
int pin2 = 13;
int pin3 = 11;
int pin4 = 8;

unsigned long previousMillis = 0;
unsigned long interval = 1000;
int a = 60;

bool ready  = true;
bool newData = false;
bool volatile isInterrupt = false;
String recData;
String temp;
int pinINT = 5;

void setup() {
  Serial.begin(115200);
  pinMode(13,   OUTPUT); //13 is the built in led, it is used fro testing. replace with pin 1 on the production
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pinINT, INPUT_PULLUP);
  attachPCINT(digitalPinToPCINT(pinINT), interrupt, FALLING);

  Serial.flush();
}

void loop() {
  isInterrupt = false;
  if (ready) {

    clearSerialBuffer();
    Serial.write("ready\n");
    receiveData();
  }

  if (recData == "0") {
    Serial.write(temp.c_str());
    Serial.write('\n');
  }
  timer();

  if (newData && !isInterrupt) {
    readJSON( const_cast<char*>(recData.c_str()) );
    temp = recData;
  }
}

void interrupt() {
  isInterrupt = true;
  sendData();
  // delay(1000);
  ready = true;
}

void receiveData() {

  while (Serial.available() == 0 && !isInterrupt) {}

  recData = Serial.readStringUntil('\n');
  newData = true;
  ready = false;
}

void timer() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    sendData();
  }
}

void sendData() {
  clearSerialBuffer();
  Serial.write("{\"1\":1,\"2\":0,\"3\":25,\"4\":11,\"5\":0,\"6\":0,\"7\":0,\"8\":0,\"9\":0,\"10\":0,\"11\":0,\"12\":0,\"13\":0,\"14\":0,\"15\":0}\n");
}

void clearSerialBuffer() {
  while (Serial.available() > 0)
    Serial.read();
}

void readJSON( const char* json ) {
  newData = false;
  ready = true;
  StaticJsonDocument < JSON_OBJECT_SIZE(15) + 40 > doc;
  DeserializationError err = deserializeJson(doc, json);

  if (err.c_str() != "Ok") {
    //delay(1000);
    //Serial.write("Failed to read json\n");
    return;
  }

  if ( doc["1"] == 0 ) {
    pinOutput(1, 0, 1, 0);
    digitalWrite(13, LOW);
    //Serial.println("The lamp is off");
  } else if ( doc["1"] == 1 ) {
    pinOutput(0, 0, 1, 0);
    digitalWrite(13, HIGH);
    //Serial.println("The lamp is on");
  }
}

void pinOutput(int a, int b, int c, int d) {
  digitalWrite(pin1, a);
  digitalWrite(pin2, b);
  digitalWrite(pin3, c);
  digitalWrite(pin4, d);
}
