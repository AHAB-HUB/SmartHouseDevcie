#include "RestClient.h"

RestClient::RestClient()
{
  httpCode = -1;
}

RestClient::~RestClient()
{
  http.end(); 
}

String RestClient::getDevices()
{
  http.begin(smartHouseURL + devicesURL);
  httpCode = http.GET();
  return (httpCode > 0)? http.getString() : " ";
}

void RestClient::putDevices(String device)
{
  http.begin(smartHouseURL + devicesURL);
  http.addHeader("Content-Type", "application/json");
  httpCode = http.sendRequest("PUT", device.c_str());
}

int RestClient::responseCode()
{   
  return httpCode;
}
