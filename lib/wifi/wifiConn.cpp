#include <Arduino.h>
#include <WiFi.h>
#include "wifiConn.h"
#include "../../settings.h"

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