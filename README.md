## Overview

This lab is an IoT application designed to run on an ESP32 microcontroller. It connects to a WiFi network, interacts with a REST API to PUT and GET data.It controls an LED's state based position of the switch on the UI website. Additionally, it reads temperature data from a sensor and sends it to the API. Which is then displayed on the UI website.

---

### Functions

1. **`getLedState(const char* _ENDPOINT, const char* _API_KEY, const char* _PATH)`**  
   - Sends an HTTP GET request to the specified endpoint with the provided API key and path.  
   - Returns the response body as a `String`. If the request fails, it returns the previous response body.

2. **`parseJSON(String _body, const char* _key)`**  
   - Parses a JSON string and extracts the value associated with the specified key.  
   - Returns the value as a `bool`. If parsing fails, it returns the previous JSON state.

3. **`setLedState(bool _ledState)`**  
   - Controls the state of an LED based on the provided boolean value.  
   - Turns the LED on if `true` and off if `false`.

4. **`readTemp()`**  
   - Reads the temperature from a sensor connected to the ESP32.  
   - Converts the analog reading to a temperature value in Celsius and returns it as a `float`.

5. **`putTemp(float _temp, const char* _ENDPOINT, const char* _API_KEY, const char* _PATH)`**  
   - Sends an HTTP PUT request to the specified endpoint with the provided API key and path.  
   - Sends the temperature data in JSON format. If the request fails, it prints an error message.

---

## Purpose

This code was written to fulfill the course requirements of **'ECSE3038 Engineering Internet of Things Systems'**. **Objective:** Apply IoT concepts using a physical ESP32 microcontroller to implement a system that interacts with a REST API, controls an LED, and reads and sends temperature data.

---

## Try It 

Ask a friend to pull up the light switch site and show them that they can remotely control your circuit over the internet.

---

![Circuit](https://i.imgur.com/YOi6eLU.jpeg)

