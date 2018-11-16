#include "ESP8266WiFi.h"
 
ADC_MODE(ADC_VCC); // vcc uitlezen.
 
void setup(void) { 
  Serial.begin(9600);
 
  float vdd = ESP.getVcc() / 1024.0;
  Serial.print("Voltage: ");
  Serial.print(vdd); 
  Serial.println("V");
}
 
void loop() {
  // Nothing
  delay(10);
}
