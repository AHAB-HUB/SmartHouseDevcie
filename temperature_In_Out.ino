#include <DallasTemperature.h>

#include <OneWire.h>

//Digital temperature (outside house)
const int pinTempOut = 9; //create the instance, sensor connected to 9

OneWire oneWire(pinTempOut); //oneWire instance to communicate device

DallasTemperature tempOut(&oneWire); //pass onewire to dallas temp sensor

//Analog Temperature ( 2 sensors inside house)
const int pinTempIn1 = A1;
const int pinTempIn2 = A2;

//SMT160 sensor: outside
unsigned int waveHighTimeCount;
unsigned int waveLowTimeCount;
float dutyCycle;
int sampleSize;

void setup(){
  Serial.begin(115200); //initialize serial monitor @baud rate
  tempOut.begin(); //initialize temp sensor
  
}
void loop(){
  //House outside temperature
  tempOut.requestTemperatures(); //call this method first b4 getting actual temp
  float tempOutCent = tempOut.getTempCByIndex(0); //index zero bcos is 1 sensor
  float tempOutFar = tempOut.getTempFByIndex(0);

  // House 1st inside temperature sensor calculated in both centimeter/ fahrenheit
  int readPin1 = analogRead(pinTempIn1) //enter pin to read
  float volts1 = readPin1 * 0.5;
  volts1 /= 1024.0;
  float tempInCent1 = (volts1 - 0.5) * 100;
  float tempInFar1 = (tempInCentq * 9.0 / 5.0) + 32.0;

  Serial.print("Inside Temperature 1st: ");
  Serial.println(tempInCent1);

  // House 2nd inside temperature sensor calculated in both centimeter/ fahrenheit
  int readPin2 = analogRead(pinTempIn2) //enter pin to read
  float volts2 = readPin2 * 0.5;
  volts2 /= 1024.0;
  float tempInCent2 = (volts2 - 0.5) * 100;
  float tempInFar2 = (tempInCent2 * 9.0 / 5.0) + 32.0;

  Serial.print("Inside Temperature 2nd: ");
  Serial.println(tempInCent2);

//Check for level of temp to trigger alarm
  if(tempInCent1 > 30){

    if(tempInCent2 <= 15){
      //trigger alarm
    }
    else{
      //No alarm
    }
    
  }else{
    // No alarm
  }

//for SMT160 sensor with duty cycle
  waveHighTimeCount = 0;
  waveLowTimeCount = 0;
  sampleSize = 0;

  do{
    sampleSize++;
    waveHighTimeCount = waveHighTimeCount + pulseIn(pinTempOut, HIGH);
    waveLowTimeCount = waveLowTimeCount + pulseIn(pinTempOut, LOW);
  }
  while(sampleSize < 200);
  
    dutyCycle = waveHighTimeCount;
    dutyCycle /= (waveHighTimeCount + waveLowTimeCount);

//cal temperature: using DC is (0.320 + 0.00470 = Temp)
    float temperature = (dutyCycle - 0,320)/ 0.00470;
    Serial.print("Temperature C: ");
    Serial.println(temperature);

    delay(5000);
}
