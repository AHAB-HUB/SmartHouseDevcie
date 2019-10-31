#include"RestClient.h"

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
  String tmp = smartHouseURL + devicesURL;
  http.begin(tmp);
  httpCode = http.GET();
  return (httpCode > 0)? http.getString() : " ";
}

void RestClient::postNewDevice(String device)
{
  
}

int RestClient::responseCode()
{
  return httpCode;
}
