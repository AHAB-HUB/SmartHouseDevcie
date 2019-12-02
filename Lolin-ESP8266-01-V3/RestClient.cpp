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
  http.begin(smartHouseURL + getDevicesURL);
  httpCode = http.GET();
  return (httpCode > 0)? http.getString() : " ";
}

void RestClient::putDevices(String device)
{
  http.begin(smartHouseURL + putDeviceURL);
  http.addHeader("Content-Type", "application/json;charset=UTF-8");
  httpCode = http.PUT(device);
}

int RestClient::responseCode()
{
  return httpCode;
}
