void initStatus(){
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
  
    pinMode(SUCCESS_STATUS_LED_PIN, OUTPUT);
    pinMode(STATUS_LED_PIN, OUTPUT);
    
    digitalWrite(SUCCESS_STATUS_LED_PIN, LOW);
    digitalWrite(STATUS_LED_PIN, LOW);
}

/**
* Beep the buzzer one time
*/
void buzzerBeep(){
    digitalWrite(BUZZER_PIN, HIGH);
    delay(300);
    digitalWrite(BUZZER_PIN, LOW);
}

/**
* Blink the status led one time
*/
void blinkStatusLED(int pin){
    digitalWrite(pin, HIGH);
    delay(300);
    digitalWrite(pin, LOW);
}
/**
* Beep the buzzer one time
*/
void statusLEDToggle(int pin, int state){
    digitalWrite(pin, state);
}

void showStatus(int status, int pattern){
    for(int i = 1; i <= pattern; i++){
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(status, HIGH);
      delay(300);
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(status, LOW);
      delay(200);
    }
}
