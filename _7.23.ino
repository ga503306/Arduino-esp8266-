
#include "ESP8266.h"
ESP8266 wifi(Serial2);
#define SSID        "hzt"
#define PASSWORD    "ga503306"

// ThingSpeak Settings
char thingSpeakAddress[] = "210.240.202.109";
int failedCounter = 0;

void setup()
{
  // Start Serial for debugging on the Serial Monitor
  Serial.begin(9600);
  //Serial.print("IP:");
 // Serial.println( wifi.getLocalIP().c_str());   
  Serial.print("setup begin\r\n");

    Serial.print("FW Version: ");
    Serial.println(wifi.getVersion().c_str());
    
    if (wifi.setOprToStation()) {
        Serial.print("to station ok\r\n");
    } else {
        Serial.print("to station err\r\n");
    }

    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");
        Serial.print("IP: ");       
        Serial.println(wifi.getLocalIP().c_str());
    } else {
        Serial.print("Join AP failure\r\n");
    }
}

void loop()
{
  
  uint8_t buffer[1024] = {0};
  // Read value from Analog Input Pin 0
  String analogValue0 = "10224076";
  if (wifi.createTCP(thingSpeakAddress, 80)) {
      Serial.println("create tcp ok\r\n");
  } else {
      Serial.println("create tcp err\r\n");
      return;
  }
  String post ="POST /testard.php HTTP/1.1 \r\nHost: 210.240.202.109\r\nConnection: close\r\n";
  post +="Content-Type: application/x-www-form-urlencoded\r\n";
  post +="Content-Length: ";
  post +=(analogValue0.length()+9);  //10224076 +9 -> 16 
  post +="\r\n\r\n";
  post +="sendthis=";
  post +=analogValue0;
  Serial.println(post);
  wifi.send((const uint8_t*)post.c_str(), post.length());

  uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
  if (len > 0) {
      Serial.println("Received:[");
      for(uint32_t i = 0; i < 200; i++) {
          Serial.print((char)buffer[i]);
      }
      Serial.println("]\r\n");
  }
  else {
    failedCounter++;
    Serial.println("Connection to ThingSpeak Failed ("+String(failedCounter, DEC)+")");   
  }
  if (wifi.releaseTCP()) {
      Serial.print("release tcp ok\r\n");
  } else {
      Serial.print("release tcp err\r\n");
  }

}
