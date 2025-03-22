#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"

String previousResponseBody=""; //store previous ResponseBody to gracefully handle HTTP error 
bool previousJson=""; //store previous JSON to gracefully handle JSON deserialization error 
float previousTemp=0.00;
const char* lightPath ="/api/light"; // define path to be added to root endpoint to GET the light state 
const char* tempPath ="/api/temp"; // define path to be added to root endpoint to PUT the temperature

String getLedState(const char* _ENDPOINT,const char* _API_KEY, const char* _PATH);//function prototypes
bool parseJSON (String _body, const char* _key);
void setLedState(bool _ledState);
float readTemp();
void putTemp(float _temp,const char* _ENDPOINT,const char* _API_KEY, const char* _PATH);



void setup()
{
  
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(TEMP_SENSOR, INPUT);

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
    String response_body = getLedState(ENDPOINT,API_KEY,lightPath); //get request body as string 
    bool ledState = parseJSON(response_body,"light"); //convert the request body to JSON and parse the "light" key to get the state of the LED 
    setLedState(ledState); 
    float currentTemperature = readTemp();
    putTemp(currentTemperature,ENDPOINT,API_KEY,tempPath);
  }

}


String getLedState(const char* _ENDPOINT,const char* _API_KEY, const char* _PATH)//get request body as string
{
  HTTPClient https; // declare http _object

  String _ENDPOINT_PATH = String(_ENDPOINT)+String(_PATH); // add the path to the root endpoint dynamically 
  https.begin(_ENDPOINT_PATH); // connect to endpoint/url
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
  JsonDocument _object; // initialize JSON _object 
  DeserializationError error = deserializeJson(_object, _body); // check for json conversion error 

  if (error) // return error if conversion fails 
  {
    bool _ERROR = previousJson; // return previous state if error occurs
    Serial.print("Deserialization failed: ");
    Serial.println(error.c_str());
    return _ERROR;
  }

  bool _boolJson = _object[_key];
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

float readTemp()
{
  //Serial.println(analogRead(TEMP_SENSOR));
  int _adcValue=analogRead(TEMP_SENSOR);
  float _VOUT = (_adcValue) * (5/1023.00);//VOUT= Vin * Vref/Vres
  float _temp = _VOUT*100; 
  Serial.print(_temp);
  Serial.println(" C");
  return _temp;
}


void putTemp(float _temp,const char* _ENDPOINT,const char* _API_KEY, const char* _PATH)
{   

  HTTPClient https; // declare http _object

  String _ENDPOINT_PATH = String(_ENDPOINT)+String(_PATH); // add the path to the root endpoint dynamically 
  https.begin(_ENDPOINT_PATH); // connect to endpoint/ at specifiec path
  https.addHeader("api-key", _API_KEY);// add api-key to header of get request

  JsonDocument _object;

  _object["temp"] = _temp;
  String _request_body;

  _object.shrinkToFit();  // optional

  serializeJson(_object, _request_body);

  int _status_code = https.PUT(_request_body);

  if (_status_code<=0)
  {
    //float _ERROR = previousTemp;
    Serial.println("An error ocured. See Status Code:");
    Serial.print(_status_code);
    https.end();
    return;
  }

  Serial.print("Status Code: ");// print stats code if no errors 
  Serial.println(_status_code);

  String _response_body= https.getString(); // get the body of the get request
  Serial.println(_response_body); // print body of requuest
  https.end();// end the https connection 

  return;

  }