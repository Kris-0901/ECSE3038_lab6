#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"

String previousResponseBody=""; //store previous ResponseBody to gracefully handle HTTP error 
bool previousJson=""; //store previous JSON to gracefully handle JSON deserialization error 
const char* lightPath ="/api/light"; // define path to be added to root endpoint to GET the light state 
const char* tempPath ="/api/temp"; // define path to be added to root endpoint to PUT the temperature

String appGet(const char* _ENDPOINT,const char* _API_KEY);//function prototypes
bool parseJSON (String _body, const char* _key);
void setLedState(bool _ledState);


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
    delay(500);
    digitalWrite(LED_PIN, LOW);
  }
    
  Serial.print("Conected to the WiFi Network with IP address: ");
  Serial.println(WiFi.localIP()); // print ip address 
      
}

void loop()
{

  if (WiFi.status() == WL_CONNECTED) // check if connected to WiFi
  {
    String response_body = appGet(ENDPOINT,API_KEY); //get request body as string 
    bool ledState = parseJSON(response_body,"light"); //convert the request body to JSON and parse the "light" key to get the state of the LED 
    setLedState(ledState); 
  }

}


String appGet(const char* _ENDPOINT,const char* _API_KEY)//get request body as string
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

bool parseJSON (String _body, const char* _key)  //convert request body to JSON.Parse the specified key then return JSON as bool
{ 
  JsonDocument object; // initialize JSON object 
  DeserializationError error = deserializeJson(object, _body); // check for json conversion error 

  if (error) // return error if conversion fails 
  {
    bool _ERROR = previousJson; // return previous state if error occurs
    Serial.print("Deserialization failed: ");
    Serial.println(error.c_str());
    return _ERROR;
  }

  bool _boolJson = object[_key];
  previousJson =_boolJson;// update the previous state

  return _boolJson; 
}

void setLedState(bool _ledState)
{
  if(_ledState)
    digitalWrite(LED_PIN,HIGH);
  else
    digitalWrite(LED_PIN,LOW);
  
    return;
}