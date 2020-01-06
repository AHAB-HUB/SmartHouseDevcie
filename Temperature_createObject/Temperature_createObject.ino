
String dataToPost;
String variableToPost = "temp=";

char server[] = "www.hkr.com";
WiFiClient client;

const int pinLDR = A3;
const int pinTempOut = 9;
const int pinTempIn = A2;
void setup(){
  
}
void loop(){

// House temperature calculated in both centimeter/ fahrenheit
  int readPin = analogRead(pinTempOut) //enter pin to read
  float volts = readPin * 0.5;
  volts /= 1024.0;
  float tempInCent = (volts - 0.5) * 100;
  float tempInFar = (tempInCent * 9.0 / 5.0) + 32.0;

  dataToPost = variableToPost + tempInFar;

  //send via POST method
  if(client.connect(server, 80){
    client.println("POST /smartHouse/devices.html HTTP/1.1");
    client.println("HOST: www.hkr.com");
    client.println("Content-type: application/text");
    client.print("Content-lenght: ");
    client.println(dataToPost.lenght());
    client.println();
    client.print(dataToPost);
  }

  if(client.connected(){
    client.stop(); 
  }
  Serial.println(dataToPost);
  delay(5000);


  // create JSON object
  if(Serial.read() == "switch") {
    DynamicJsonBuffer djBuffer;
    JsonObject& rootObject = djBuffer.createObject();

    rootObject["switch1"] = "Light";

    String lightOn = "on";
     String lightOff = "off";
    rootObject["switch2"] = lightOn;
    rootObject["switch3"] = lightOff;
    
  }
  
}



 
