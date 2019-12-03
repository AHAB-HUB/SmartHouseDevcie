#ifndef RestClient_H
#define RestClient_H

#include <ESP8266HTTPClient.h>

class RestClient{

  public:
    RestClient();
    ~RestClient();
    String getDevices();
    void putDevices(String devices);
    int responseCode();
    
    //10.0.0.2
  private:
    HTTPClient http;
    int httpCode; 
    String payload;
    const String smartHouseURL = "http://192.168.50.203:8080/SmartHouseApi";
    const String devicesURL = "/devices";
};
#endif // !RestClient_H
