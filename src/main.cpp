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
StaticJsonDocument<1024> doc;
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
    // char hostName[] = HOST_NAME;
    // int httpPort = HTTP_PORT;
    // char seZone[6] = SEZONE;

    //#define HOST_NAME "https://www.elprisetjustnu.se/api/v1/prices/2023/12-06_SE3.json"
    //#define HOST_NAME "https://www.elprisetjustnu.se/api/v1/prices/"

    // char curDate[25];
    // strcpy(curDate, curYear);
    // strcat(curDate, "/");
    // strcat(curDate, curMonth);
    // strcat(curDate, "-");
    // strcat(curDate, curDay);
    // strcat(curDate, "_");
    // strcat(curDate, seZone);
    // strcat(curDate, ".json");
    // strcat(hostName, curDate);

    //char question[100] = "{\"query\": \"{ viewer { name }}\"}";
    char question[200] = "{\"query\": \"{ viewer {homes {currentSubscription {priceInfo {today {total energy tax startsAt }}}}}}\"}";

    //String json = webserverRequest(question); // Demands more memory due to duplication
    const char * json = webserverRequest(question);
    tft.println(json);

    // We get an array with several objects [] = array, {} = object
    // Ref: https://arduinojson.org/v6/assistant
    DeserializationError error = deserializeJson(doc, json);

    tft.println("Got data");
    if (error) {
      tft.println("deserializeJson() misslyckades: ");
      tft.println(error.c_str());

    }
    // Hämta data från JSON-dokumentet
    float total = doc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"]["today"][0]["total"]; // 1.3642
    float energy = doc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"]["today"][0]["energy"]; // 0.9938
    float tax = doc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"]["today"][0]["tax"]; // 0.3704
    const char* startsAt = doc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"]["today"][0]["startsAt"]; 
    tft.println("Total: ");
    total = doc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"]["today"][1]["total"];
    tft.println("Total: ");


    previousMillis = currentMillis;
  }

}
