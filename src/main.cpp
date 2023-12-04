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
  // #ifdef DEBUG
  //   Serial.begin(115200);
  //   delay(1000);
  //   Serial.printf("Welcome to %s!\n", APPNAME);
  // #endif



  Serial.begin();
  //Serial1.begin(9600);
  //Serial2.begin(9600);
  //while (!Serial); 

  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(30, 30, 4);
  tft.setTextColor(TFT_SKYBLUE);
  tft.println("BOOT");

  Serial.println("This is port \"Serial\"");

  char *ipAddrPtr = ipAddr;
  wifi_connect(ipAddrPtr);

//  Serial.printf("OK");
  tft.println("OK");
  tft.println(ipAddr);

  tft.println("Get time");
  //(void)getInternetTime(mytime); 
  (void)getInternetTime(); 
  tft.println("Time ok");
  delay(2500);

}

void loop() {

  //tft.setCursor(0, 230, 4);
 // tft.print(asctime(&mytime.timeinfo));

  //tft.drawRect(0,0,100, 10, 3);
  // for (int i = 0; i < 40; i++)
  // {
  //   int rx = random(60);
  //   int ry = random(60);
  //   int x = rx + random(480 - rx - rx);
  //   int y = ry + random(320 - ry - ry);
  //   tft.fillEllipse(x, y, rx, ry, random(0xFFFF));
  // }
  long int ms = millis();
  tft.fillRect(0,0,320,100,0x000); // 320 = width
  tft.setCursor(11, 11, 4);

    datetime_t t = {
            .year  = 2020,
            .month = 06,
            .day   = 05,
            .dotw  = 5, // 0 is Sunday, so 5 is Friday
            .hour  = 15,
            .min   = 45,
            .sec   = 00
    };

  char datetime_buf[256];
  char *datetime_str = &datetime_buf[0];
  rtc_get_datetime(&t);
  datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
  char curTime[25];
  sprintf(curTime, "%02d:%02d:%02d", t.hour, t.min, t.sec);

  tft.println(curTime);
  delay(1000);
}
