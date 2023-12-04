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

TFT_eSPI tft = TFT_eSPI();
WiFiClient wifiClient;
//mytime_t mytime;

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
}
