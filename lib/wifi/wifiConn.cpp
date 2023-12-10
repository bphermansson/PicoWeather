#include <Arduino.h>
#include <WiFi.h>
#include "wifiConn.h"
#include "../../settings.h"
#include <WiFiClientSecureBearSSL.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

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

String webserver_connect(char hostName[], int httpPort) {

  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;

    if (https.begin(*client, hostName)) {  // HTTPS
      int httpCode = https.GET();
        if (httpCode == 200) {
          String res = https.getString();
          char *payload;
          res.toCharArray(payload, sizeof(payload));
          https.end();
        return res;
        }
      return "Error";
    }
  return "666";
}