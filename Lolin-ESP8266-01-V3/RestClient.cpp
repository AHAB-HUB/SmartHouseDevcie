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

void RestClient::postDevices(String devices)
{
  
}

int RestClient::responseCode()
{
  return httpCode;
}
