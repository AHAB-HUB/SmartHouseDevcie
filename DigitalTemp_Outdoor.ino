#include <OneWire.h>
#include <DallasTemperature.h>


const int pinTempOut = 9; //create the instance, sensor connected to 9

OneWire oneWire(pinTempOut); //oneWire instance to communicate device

DallasTemperature tempOut(&oneWire); //pass onewire to dallas temp sensor

void setup(){
  Serial.begin(115200); //initialize serial monitor @baud rate
  tempOut.begin(); //initialize temp sensor
}
void loop(){
  tempOut.requestTemperatures(); //call this method first b4 getting actual temp
  float tempInCent = tempOut.getTempCByIndex(0); //index zero bcos is 1 sensor
  float tempInFar = tempOut.getTempFByIndex(0);
}
