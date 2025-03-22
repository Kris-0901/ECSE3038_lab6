#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"

String previousResponseBody="";
const char* previousMessage="";
const char* lightPath ="/api/light"; // define path to be added to root endpoint to GET the light state 
const char* tempPath ="/api/temp"; // define path to be added to root endpoint to PUT the temperature

String appGet(const char* _ENDPOINT,const char* _API_KEY);//function prototypes
const char* parseJSON (String _message, const char* _key);


void setup()
{
  
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  if (IS_WOKWI) 
      WiFi.begin(SSID, PASS, CHANNEL);
  else 
      WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) // wait for WiFi connection
  {
    delay(500);
    Serial.print(".");
  }
    
  if ( WiFi.status() == WL_CONNECTED) {// turn on built in Led briefly when connected to wifi 
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
  }
    
  Serial.print("Conected to the WiFi Network with IP address: ");
  Serial.println(WiFi.localIP()); // print ip address 
      
}

void loop()
{

  if (WiFi.status() == WL_CONNECTED)
  {
    String response_body = appGet(ENDPOINT,API_KEY); //get request body as string 
  }

}


String appGet(const char* _ENDPOINT,const char* _API_KEY)
{
  HTTPClient https; // declare http object
  
  String PATH = String(_ENDPOINT)+String(lightPath); // add the path to the root endpoint dynamically 
  https.begin(PATH); // connect to endpoint/url
  https.addHeader("api-key", _API_KEY);// add api-key to header of get request


  int _responseCode=https.GET();// get the RestAPI status code

  if (_responseCode<=0){// check for errors 
    String _ERROR = previousResponseBody;
    Serial.print("An error occured with repsonse code:");
    Serial.println(_responseCode);
    https.end();
    return _ERROR;
  }

  Serial.print("Status Code: ");// print stats code if no errors 
  Serial.println(_responseCode);

  String _response_body= https.getString(); // ge the body of the get request
  previousResponseBody=_response_body;
  Serial.println(_response_body); // print body of requuest
  https.end();// end the https connection 

  return _response_body;
}

const char* parseJSON (String _message, const char* _key)  //convert request body to JSON.Parse the specified key then return message as char*
{ 
  JsonDocument object; // initialize JSOn object 
  DeserializationError error = deserializeJson(object, _message); // check for json conversion error 

  if (error) // return errror if conversion fails 
  {
    const char* _ERROR = previousMessage;
    Serial.print("Deserialization failed: ");
    Serial.println(error.c_str());
    return _ERROR;
  }

  const char* _charMessage = object[_key];
  previousMessage =_charMessage;

  return _charMessage; 
}