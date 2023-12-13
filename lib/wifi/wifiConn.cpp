#include <Arduino.h>
#include <WiFi.h>
#include "wifiConn.h"
#include "../../settings.h"
#include <WiFiClientSecureBearSSL.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "../../token.h"

WiFiClient client;

void wifi_connect(char *ipAddrPtr)
{   
  WiFi.begin(MYSSID, PASSWORD);  
  NTP.begin("pool.ntp.org", "time.nist.gov");

  #ifdef DEBUG
    Serial.print("Connecting to Wifi");
  #endif

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    #ifdef DEBUG
      Serial.print(".");
    #endif
  } 

  char IP[] = "xxx.xxx.xxx.xxx";          // buffer
  IPAddress ip = WiFi.localIP();
  ip.toString().toCharArray(IP, 16);
  strcpy(ipAddrPtr, IP);
}

const char * webserverRequest(char question[]) {
  String tibberUrl = TIBBERURL;
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;

    if (https.begin(*client, tibberUrl)) {  
      String tknSettings = TOKEN;
      String tkn = "Bearer " + tknSettings;
      https.addHeader("Authorization", tknSettings) ;
      https.addHeader("Content-Type", "application/json");
      int httpCode = https.POST(question);

        if (httpCode == 200) {
          String res = https.getString();
          https.end();
          const char* buf = res.c_str();
          return buf;
        }
      return "Error";
    }
  return "666";
}