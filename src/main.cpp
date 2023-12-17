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
const long interval = 80000; 
StaticJsonDocument<3072> doc;
float curPrices[24];

struct allPricesStruct {
  float price;
  int hour;
};

void getPrices();
void sort(int n, allPricesStruct* ptr);  


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

  getPrices();
}

void loop() {
  long int ms = millis();
  tft.fillRect(0,0,320,30,0x000); // 320 = width
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
    // char curYear[5], curMonth[5], curDay[5];  
    // sprintf(curYear, "%02d", t.year);
    // sprintf(curMonth, "%02d", t.month);
    // sprintf(curDay, "%02d", t.day);
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

    
    // Hämta data från JSON-dokumentet
    // float total = doc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"]["today"][0]["total"]; // 1.3642
    // float energy = doc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"]["today"][0]["energy"]; // 0.9938
    // float tax = doc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"]["today"][0]["tax"]; // 0.3704
    // const char* startsAt = doc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"]["today"][0]["startsAt"]; 
//    tft.print("Total: ");
//    tft.println(total);




    // float curPrice = doc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"]["today"][t.hour]["total"];
    // tft.print("Now: ");
    // tft.println(curPrice);




    previousMillis = currentMillis;
  }

}

void getPrices(){
    char questionToday[200] = "{\"query\": \"{ viewer {homes {currentSubscription {priceInfo {today {total energy tax startsAt }}}}}}\"}";

    // Ref: https://arduinojson.org/v6/assistant

    DeserializationError error = deserializeJson(doc, webserverRequest(questionToday));
    tft.println("Got data for today");
    if (error) {
      tft.println("deserializeJson() misslyckades: ");
      tft.println(error.c_str());
    }

    

    allPricesStruct allPrices[48];

    int j;
    for(j=0;j<24;j++){
      allPrices[j].price = doc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"]["today"][j]["total"];
      String time = doc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"]["today"][j]["startsAt"];
        char buf[11];
        time.toCharArray(buf, 25);
        char* ptr = strtok(buf, "T");
        ptr = strtok(NULL, ":");
        allPrices[j].hour = atoi(ptr);
    }

    // char questionTomorrow[200] = "{\"query\": \"{ viewer {homes {currentSubscription {priceInfo {tomorrow {total energy tax startsAt }}}}}}\"}";
    // error = deserializeJson(doc, webserverRequest(questionTomorrow));
    // tft.println("Got data for tomorrow");
    // if (error) {
    //   tft.println("deserializeJson() misslyckades: ");
    //   tft.println(error.c_str());
    // }

    // int c=0;
    // for(j=j;j<48;j++){
    //   allPrices[j] = doc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"]["today"][j]["total"];
    //   if (allPrices[j].price == 0) {
    //     c++;
    //   }
    // }
    // tft.println(c);


     allPricesStruct *ptrValues;
     ptrValues = allPrices;

      sort(10, ptrValues);  
      tft.print(ptrValues[0].hour);
      tft.println(ptrValues[0].price);

    // for(int k=0;k<48;k++){
    //   //tft.println(allPrices[k]);
    // }

    // int cheapTime=0;
    // int cheapestHour=allPrices[0];
    // float values[48];
    // for(int i = 0; i<30; i++) { // Do we always have 20 values? No.

    //   if(allPrices[i] < cheapestHour) {
    //     cheapestHour = allPrices[i];
    //   }

    //   float a = allPrices[i]; 
    //   float b = allPrices[i+1];
    //   float c = allPrices[i+2];
    //   if(a>0 && b > 0 && c>0) {
    //     float meanValue = (a+b+c)/3;
    //       if (meanValue<1) {
    //         cheapTime = i;
    //         break;
    //       }
    //   }
    // }
    // if(cheapTime>0) {
    //     tft.println(cheapTime);
    // }
    // else {
    //   tft.println("No cheap time today");
    //   tft.println("Cheapest: ");
    //   tft.println(cheapestHour);
    // }
}
void sort(int n,  allPricesStruct *allPrices)  
{  
    int i, j, t;  
    float *ptr;
    float price = allPrices->price;
    *ptr = price;
    // Sort the numbers using pointers  
    for (i = 0; i < n; i++) {  
        for (j = i + 1; j < n; j++) {  
            if (*(ptr + j) < *(ptr + i)) {  
                t = *(ptr + i);  
                *(ptr + i) = *(ptr + j);  
                *(ptr + j) = t;  
            }  
        }  
    }  
  
    // print the numbers  
    // for (i = 0; i < n; i++)  
    //     printf("%d ", *(ptr + i));  
}  