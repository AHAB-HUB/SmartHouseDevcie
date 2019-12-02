#include <PinChangeInterrupt.h>
#include <ArduinoJson.h>
#include "Device.h"

/*
   This is the main class to handle the arduino board functionalities
   and to implement them in a specific order. These functionalities are as such as:
   - Retriving data from the esp8266 board
   - translate this data and converte it into states
   - Apply these states into the connected devices with arduino
*/

//Pins
int pin1 = 12;
int pin2 = 13;
int pin3 = 11;
int pin4 = 8;
int pinINTFireAlarm = 2;
int pinINTBurglarAlarm = 3;
int pinINTWaterLeakage = 4;
int pinINTStove = 5;
int pinINTWindow = 6;

//Timer's attributes
unsigned long volatile previousMillis = 0;
unsigned long interval = 2000;

//Booleans
bool ready  = true;
bool newData = false;
bool volatile isInterrupt = false;
// Strings
String recData;
String temp;
String seData;
// int
int volatile fireAlarm = 0;
int volatile burglarAlarm = 0;
int volatile waterLeakage = 0;
int volatile stove = 0;
int volatile window = 0;
int volatile power = 0;
int volatile lightSensor = 0;

void setup() {
  Serial.begin(115200);
  pinMode(13,   OUTPUT); //13 is the built in led, it is used fro testing. replace with pin 1 on the production
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);

  pinMode(pinINTFireAlarm, INPUT_PULLUP);
  pinMode(pinINTBurglarAlarm, INPUT_PULLUP);
  pinMode(pinINTWaterLeakage, INPUT_PULLUP);
  pinMode(pinINTStove, INPUT_PULLUP);
  pinMode(pinINTWindow, INPUT_PULLUP);

  attachPCINT(digitalPinToPCINT(pinINTFireAlarm), interrupt, FALLING);
  attachPCINT(digitalPinToPCINT(pinINTBurglarAlarm), interrupt, FALLING);
  attachPCINT(digitalPinToPCINT(pinINTWaterLeakage), interrupt, FALLING);
  attachPCINT(digitalPinToPCINT(pinINTStove), interrupt, FALLING);
  attachPCINT(digitalPinToPCINT(pinINTWindow), interrupt, FALLING);

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

  if (newData && !isInterrupt) {
    readJSON( const_cast<char*>(recData.c_str()) );
    temp = recData;
  }
  timer();
}

void interrupt() {
  previousMillis = millis(); // to prevent the timer to interfere with the interrupt
  isInterrupt = true;
  fireAlarm = digitalRead(pinINTFireAlarm);
  burglarAlarm = digitalRead(pinINTBurglarAlarm);
  waterLeakage = digitalRead(pinINTWaterLeakage);
  stove = digitalRead(pinINTStove);
  window = digitalRead(pinINTWindow);
  sendData();
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
  buildStringFormat();
  Serial.write(seData.c_str());
  Serial.write('\n');
}

void buildStringFormat(){
    seData  = "{\"6\":";
    seData += power;
    seData += ",\"7\":"; 
    seData += fireAlarm; 
    seData += ",\"8\":";
    seData += burglarAlarm;
    seData += ",\"10\":";
    seData += waterLeakage;
    seData += ",\"11\":";
    seData += stove;
    seData += ",\"12\":";
    seData += window;
    seData += ",\"15\":";
    seData += lightSensor;
    seData += "}";
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
