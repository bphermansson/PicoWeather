/**
 * @file main.cpp
 * @author Patrik Hermansson (hermansson.patrik@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-28
 * 
 * @copyright (c) Paheco 2023
 * 
 */

#include "../settings.h"
#include <Arduino.h>
#include <TFT_eSPI.h> 
#include "Free_Fonts.h"
#include "wifiConn.h"
#include "main.h"
#include "internetTime.h"
#include <hardware/rtc.h>
#include "pico/util/datetime.h"
#include <TimeLib.h>
#include <ArduinoJson.h>

TFT_eSPI tft = TFT_eSPI();
WiFiClient wifiClient;

unsigned long previousMillis = 0;
const long interval = 8000; 
DynamicJsonDocument doc(3072);
float curPrices[24];

void setup() {
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(30, 30, 4);
  tft.setTextColor(TFT_SKYBLUE);
  tft.println("BOOT");

  char *ipAddrPtr = ipAddr;
  wifi_connect(ipAddrPtr);

  tft.println("OK");
  tft.println(ipAddr);

  tft.println("Get time");
  (void)getInternetTime(); 
  tft.println("Time ok");
  delay(2500);
  tft.fillRect(0,0,320,200,0x000); // 320 = width
}

void loop() {
  long int ms = millis();
  tft.fillRect(0,0,320,20,0x000); // 320 = width
  tft.setCursor(11, 11, 4);

  char *datetime_str = &datetime_buf[0];
  rtc_get_datetime(&t);
  datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
  char curTime[25];
  sprintf(curTime, "%02d:%02d:%02d", t.hour, t.min, t.sec);

  tft.println(curTime);
  delay(1000);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    char curYear[5], curMonth[5], curDay[5];  
    sprintf(curYear, "%02d", t.year);
    sprintf(curMonth, "%02d", t.month);
    sprintf(curDay, "%02d", t.day);
    char hostName[] = HOST_NAME;
    int httpPort = HTTP_PORT;
    char seZone[6] = SEZONE;

    //#define HOST_NAME "https://www.elprisetjustnu.se/api/v1/prices/2023/12-06_SE3.json"
    //#define HOST_NAME "https://www.elprisetjustnu.se/api/v1/prices/"

    char curDate[25];
    strcpy(curDate, curYear);
    strcat(curDate, "/");
    strcat(curDate, curMonth);
    strcat(curDate, "-");
    strcat(curDate, curDay);
    strcat(curDate, "_");
    strcat(curDate, seZone);
    strcat(curDate, ".json");
    strcat(hostName, curDate);

    String t = webserver_connect(hostName, httpPort);
    tft.println(hostName);

    // We get an array with several objects [] = array, {} = object
    // Ref: https://arduinojson.org/v6/assistant
    DeserializationError error = deserializeJson(doc, t);

    tft.println("Got data");
    if (error) {
      tft.print(F("deserializeJson() failed: "));
      tft.println(error.f_str());
      return;
    }
    int hourNow=0;
    for (JsonObject item : doc.as<JsonArray>()) {
      float SEK_per_kWh = item["SEK_per_kWh"]; // 1.0862, 1.05081, 1.05081, 1.0176, 1.0128, 0.49048, 0.84501, ...
      curPrices[++hourNow] = item["SEK_per_kWh"];
      float tme = item["SEK_per_kWh"];
      tft.print("tme");
      tft.print(tme);
      //float EUR_per_kWh = item["EUR_per_kWh"]; // 0.09516, 0.09206, 0.09206, 0.08915, 0.08873, 0.04297, ...
      //double EXR = item["EXR"]; // 11.414425, 11.414425, 11.414425, 11.414425, 11.414425, 11.414425, ...
      const char* time_start = item["time_start"]; // "2023-11-22T00:00:00+01:00", ...
      const char* time_end = item["time_end"]; // "2023-11-22T01:00:00+01:00", "2023-11-22T02:00:00+01:00", ...
    }
      tft.print(curPrices[5]);

    for (int i=0; i<sizeof curPrices/sizeof curPrices[0]; i++) {
      tft.print(curPrices[i]);
      tft.print("-");
    }
    previousMillis = currentMillis;
  }

}
