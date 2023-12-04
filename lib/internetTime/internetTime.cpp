#include <Arduino.h>
#include "internetTime.h"
#include "../../settings.h"
#include <WiFi.h>
#include <time.h>
#include <hardware/rtc.h>

//void getInternetTime(mytime_t& mytime)
void getInternetTime()
{
    NTP.begin(NTPSERVER1, NTPSERVER2);
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }
    
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    int year = timeinfo.tm_year + 1900;
    int month = timeinfo.tm_mon + 1;
    int day = timeinfo.tm_mday;
    int hour = timeinfo.tm_hour + timeinfo.tm_isdst + TZ;
    int minute = timeinfo.tm_min;
    int seconds = timeinfo.tm_sec;
    int dayOfWeek = timeinfo.tm_wday;
    
    rtc_init();
    datetime_t currTime[7] = { year, month, day, dayOfWeek, hour, minute, seconds };
    rtc_set_datetime(currTime);
    sleep_us(64);
}
