#include <Arduino.h>
#include <rdm6300.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define WPF_SWITCH_PIN 2 //D4
#define BUZZER_PIN 5 //D1

#define SUCCESS_STATUS_LED_PIN 4 //D2
#define STATUS_LED_PIN 0 //D3

#define RFID_RX 14 //D5

#define DEBUG true
#define SERVER_IP "http://prod-trackable.ap-southeast-1.elasticbeanstalk.com"

StaticJsonDocument<200> doc;

char mac[12];
Rdm6300 rdm6300;

void setup() {

  if(DEBUG) Serial.begin(9600);
  if(DEBUG) Serial.println("Initializing");
   
  initStatus();
  statusLEDToggle(STATUS_LED_PIN, HIGH);

  connectToAP();
  initiateWPSConfigIfNessesary();
    
  rdm6300.begin(RFID_RX);
  showStatus(STATUS_LED_PIN, 2);


  unsigned char macBuff[6];
  WiFi.macAddress(macBuff);
  sprintf(
      mac,  
      "%02X:%02X:%02X:%02X:%02X:%02X",
      macBuff[0], macBuff[1], macBuff[2], macBuff[3], macBuff[4], macBuff[5]
  );
  if(DEBUG) Serial.printf("MAC: %s\n\n", mac);


  if(DEBUG) Serial.println("Successfully initialized.");
}

void loop() {

    if (rdm6300.get_new_tag_id()){
      showStatus(STATUS_LED_PIN, 1);
    
    if(DEBUG) Serial.printf("\nCARD ID: %d\n", rdm6300.get_tag_id());
    
    markAttendance(String(rdm6300.get_tag_id()));
  }


  delay(10);
}

void markAttendance(String tagKey) {
    WiFiClient client;
    HTTPClient http;

    if(DEBUG) Serial.println("[HTTP] Begin...");
    
    // configure client and url
    http.begin(client, SERVER_IP "/api/tags/mark/"); //HTTP
    http.addHeader("Content-Type", "application/json");

    if(DEBUG) Serial.println("[HTTP] POST...");
    // start connection and send HTTP header and body

   
    char payloadBuff[255];
    sprintf(payloadBuff, "{\"tag_id\": \"%s\", \"device_mac\": \"%s\"}\n", tagKey, mac);

    if(DEBUG) Serial.printf("Payload: %s", payloadBuff);
    int httpCode = http.POST(payloadBuff);
    String response = http.getString();
    
  
    
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      if(DEBUG) Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      if(DEBUG) Serial.printf("Response: %s\n", response);
      
      if(httpCode == 201){
        //success. attendance marked
        showStatus(SUCCESS_STATUS_LED_PIN, 1);
      }else{
        //error occured (API reject the RFID key)
        showStatus(STATUS_LED_PIN, 2);
      }
    } 
    
    http.end();
    
    // httpCode will be negative on error
    if (httpCode < 1) {
       if(DEBUG) Serial.printf("[HTTP] POST... Failed, error: %s\n", http.errorToString(httpCode).c_str());

      // Unknown error occured. 
      showStatus(STATUS_LED_PIN, 5);
    }
}
