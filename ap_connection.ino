void waitTillWPSInitiate(){
    pinMode(WPF_SWITCH_PIN, INPUT_PULLUP);
    while(digitalRead(WPF_SWITCH_PIN))
      yield(); // do nothing
}

void connectToAP(){
    if(DEBUG){
      Serial.print("Connecting.");
    }
  
    WiFi.mode(WIFI_STA);
    WiFi.begin(WiFi.SSID().c_str(),WiFi.psk().c_str()); // reading data from EPROM, 
    
    while (WiFi.status() == WL_DISCONNECTED) {          // last saved credentials
      delay(500);
      if (DEBUG) Serial.print(".");
    }
}


bool startWPSPBC() {
    if(DEBUG){
      Serial.println("WPS config start");
    }
    
    bool wpsSuccess = WiFi.beginWPSConfig();
    
    if(wpsSuccess) {
        String newSSID = WiFi.SSID();
        
        if(newSSID.length() > 0) {
          // WPSConfig has already connected in STA mode successfully to the new station. 
          Serial.printf("Connected successfull to SSID '%s'\n", newSSID.c_str());
        } else {
          wpsSuccess = false;
        }
    }
    
    return wpsSuccess; 
}

/**
 * Initiate WPS config if it is nessasary.
 */
void initiateWPSConfigIfNessesary(){
  wl_status_t status = WiFi.status();
  if(status == WL_CONNECTED) {
    if (DEBUG) Serial.printf("\nConnected successful to SSID '%s'\n", WiFi.SSID().c_str());
  } else {
    // WPS button I/O setup
    showStatus(STATUS_LED_PIN, 3);
    statusLEDToggle(STATUS_LED_PIN, HIGH);
    
    if(DEBUG){
      Serial.println("\nCould not connect to WiFi.\n");
      Serial.println("Please press WPS button on your router (WPS timeout = 2 minutes).");
    }
    
    waitTillWPSInitiate();  // wait for WPS Button active
    showStatus(STATUS_LED_PIN, 1);
    statusLEDToggle(STATUS_LED_PIN, HIGH);
 
    if(!startWPSPBC()) {
        if(DEBUG) Serial.println("Failed to connect with WPS");  
    } else {
      WiFi.begin(WiFi.SSID().c_str(),WiFi.psk().c_str()); // reading data from EPROM, 
      while (WiFi.status() == WL_DISCONNECTED) {          // last saved credentials
        delay(500);
        if(DEBUG) Serial.print("."); // show wait for connect to AP
      }
      statusLEDToggle(STATUS_LED_PIN, LOW);
    }
  }   
}
