#include <OneWire.h>
#include <DallasTemperature.h>

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

int oldvalueoutdoorLamp = 0;
//SMT160 sensor: outside
unsigned int waveHighTimeCount;
unsigned int waveLowTimeCount;
float dutyCycle;
int sampleSize;

//Pins
int pin1 = 12;
int pin2 = 13;
int pin3 = 11;
int pin4 = 8;
int pin7 = 7;
int pinTempIn1 = A1;//Analog Temperature ( 2 sensors inside house)
int pinTempIn2 = A2;
int pinINTFireAlarm = 2;
int pinINTBurglarAlarm = 3;
int pinINTWaterLeakage = 4;
int pinINTStove = 5;
int pinINTWindow = 6;
int pinTempOut = 9; //create the instance, sensor connected to 9
int pinFan = 10;
//Timer's attributes
unsigned long volatile previousMillis = 0;
unsigned long interval = 2000;

//Booleans
bool ready  = true;
bool newData = false;
bool volatile isInterrupt = false;
// Strings
String recData;
String seData;

// int
bool volatile fireAlarm ;
bool volatile burglarAlarm;
bool volatile waterLeakage ;
bool volatile stove ;
bool volatile window ;
bool volatile power;
int volatile LDRValue;

bool oldbool = true;
bool daylight;
int ldrStore = 0;
int ldrSensor = A3;

int old1;
int old2;
int old5;
int old6;
int old9;
int old13;
int old14;
int isAlarm = true;
bool initialize = true;

OneWire oneWire(pinTempOut); //oneWire instance to communicate device
DallasTemperature tempOut(&oneWire); //pass onewire to dallas temp sensor
float indoorTemp;
float outdoorTemp;

void setup() {
  Serial.begin(115200);
  tempOut.begin(); //initialize temp sensor
  pinMode(pin1,   OUTPUT);
  pinMode(pin2,   OUTPUT);
  pinMode(pin3,   OUTPUT);
  pinMode(pin4,   OUTPUT);
  pinMode(ldrSensor, INPUT);
  pinMode(pinFan, OUTPUT);
  pinMode (pin7,  INPUT); //power

  pinMode(pinINTFireAlarm, INPUT);
  pinMode(pinINTBurglarAlarm, INPUT);
  pinMode(pinINTWaterLeakage, INPUT);
  pinMode(pinINTStove, INPUT);
  pinMode(pinINTWindow, INPUT);

  attachInterrupt(digitalPinToInterrupt(pinINTFireAlarm), interrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinINTBurglarAlarm), interrupt, CHANGE);

  attachPCINT(digitalPinToPCINT(pinINTWaterLeakage), interrupt, CHANGE);
  attachPCINT(digitalPinToPCINT(pinINTStove), interrupt, CHANGE);
  attachPCINT(digitalPinToPCINT(pinINTWindow), interrupt, CHANGE);

  Serial.flush();
}

void loop() {
  isInterrupt = false;

  if (ready) {
    clearSerialBuffer();
    delay(100);
    Serial.write("ready\n");
    receiveData();
  }

  if (newData && !isInterrupt) {
    readJSON( const_cast<char*>(recData.c_str()) );
    LDRSensor();
      alarm();
    temperature();
    powerCheck();
    timer();
  }
}

void interrupt() {
  cli();
  previousMillis = millis(); // to prevent the timer to interfere with the interrupt
  isInterrupt = true;
  isAlarm = true;
  sendData();
  ready = true;
  yield();

  sei();
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
  powerCheck();
  buildStringFormat();
  Serial.write(seData.c_str());
  Serial.write('\n');
}

void buildStringFormat() {
  seData  = "{\"3\":";
  seData +=  indoorTemp;
  seData += ",\"4\":";
  seData += outdoorTemp;
  seData += ",\"6\":";
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
  seData += LDRValue;
  seData += "}";
}

void clearSerialBuffer() {
  while (Serial.available() > 0)
    Serial.read();
}

void initialSetup(StaticJsonDocument < JSON_OBJECT_SIZE(15) + 40 > doc) {
  old1 = doc["1"];
  old2 = doc["2"];
  old5 = doc["5"];
  old6 = doc["6"];
  old9 = doc["9"];
  old13 = doc["13"];
  old14 = doc["14"];
}

void readJSON( const char* json ) {
  newData = false;
  ready   = true;
  StaticJsonDocument < JSON_OBJECT_SIZE(15) + 40 > doc;
  DeserializationError err = deserializeJson(doc, json);
  // {"1":0,"2":0,"3":0,"4":0,"5":0,"6":0,"7":0,"8":0,"9":0,"10":0,"11":0,"12":0,"13":0,"14":0,"15":0}

  initialSetup(doc);

  if (err.c_str() != "Ok") {
    return;
  }

  //indoor lamp
  if ( doc["1"] == 0   ) {
    pinOutput(1, 0, 1, 0);
  } else if ( doc["1"] == 1 ) {
    pinOutput(0, 0, 1, 0);
  }
  //outdoor lamp
  if ((oldvalueoutdoorLamp != doc["2"])) {
    oldvalueoutdoorLamp = doc["2"];

    if ( doc["2"] == 0) {
      pinOutput(1, 1, 1, 1);
    } else if ( doc["2"] == 1 ) {
      pinOutput(0, 1, 1, 1);
    }
  }
  //radiator
  if ( doc["5"] == 0 ) {
    pinOutput(1, 1, 1, 0);
    pinOutput(1, 1, 0, 1);
  } else if ( doc["5"] == 1 ) {
    pinOutput(0, 1, 0, 1);
    pinOutput(0, 1, 1, 0);
  }  //fan
  String b = doc["9"];

  analogWrite(pinFan, b.toInt());
  //timer 1
  if ( doc["13"] == 0 ) {
    pinOutput(1, 1, 0, 0);
  } else if ( doc["13"] == 1 ) {
    pinOutput(0, 1, 0, 0);
  }
  //timer 2
  if ( doc["14"] == 0  ) {
    pinOutput(1, 0, 0, 1);
  } else if ( doc["14"] == 1 ) {
    pinOutput(0, 0, 0, 1);
  }
}

void alarm() {
  if  (isAlarm) {
    isAlarm = false;
    if  ((fireAlarm | stove | waterLeakage | window | burglarAlarm )) {
      pinOutput(1, 0, 0, 0);
      pinOutput(0, 0,  1, 1); //led on
    } else {
      pinOutput(0, 0, 0, 0);
      pinOutput(1, 0, 1, 1); //led off
    }
  }


  
}

void LDRSensor() {
  //LDR
  ldrStore = analogRead(ldrSensor);
  daylight = (ldrStore < 200);

  if  (oldbool != daylight) {
    oldbool = daylight;
    if (daylight) {
      LDRValue = 1;
      pinOutput(0, 1, 1, 1);
    } else {
      LDRValue = 0;
      pinOutput(1, 1, 1, 1);
    }
  }
}

void temperature() {

  // House 1st inside temperature sensor calculated in both centimeter/ fahrenheit
  int readPin1 = analogRead(pinTempIn1); //enter pin to read
  indoorTemp = (5.0 * readPin1 * 1000.0) / (1024 * 10);

  // House 2nd inside temperature sensor calculated in both centimeter/ fahrenheit
  //  int readPin2 = analogRead(pinTempIn2) //enter pin to read
  //  float volts2 = readPin2 * 0.5;
  //
  //  volts2 /= 1024.0;
  //  float tempInCent2 = (volts2 - 0.5) * 100;
  //  float tempInFar2  = (tempInCent2 * 9.0 / 5.0) + 32.0;

  //for SMT160 sensor with duty cycle
  waveHighTimeCount = 0;
  waveLowTimeCount = 0;
  sampleSize = 0;

  do {
    sampleSize++;
    waveHighTimeCount = waveHighTimeCount + pulseIn(pinTempOut, HIGH);
    waveLowTimeCount = waveLowTimeCount   + pulseIn(pinTempOut, LOW);
  }
  while (sampleSize < 200);

  dutyCycle = waveHighTimeCount;
  dutyCycle /= (waveHighTimeCount + waveLowTimeCount);

  //cal temperature: using DC is (0.320 + 0.00470 = Temp)
  outdoorTemp = (dutyCycle - 0.320) / 0.00470;
}

void powerCheck() {

  fireAlarm    = digitalRead(pinINTFireAlarm);
  burglarAlarm = !digitalRead(pinINTBurglarAlarm);
  waterLeakage = digitalRead(pinINTWaterLeakage);
  stove        = digitalRead(pinINTStove);
  window       = digitalRead(pinINTWindow);
}

void pinOutput(int a, int b, int c, int d) {
  digitalWrite(pin1, a);
  digitalWrite(pin2, b);
  digitalWrite(pin3, c);
  digitalWrite(pin4, d);
  delay(75);
}
