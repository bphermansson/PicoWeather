// Ref: https://werner.rothschopf.net/202011_arduino_esp8266_ntp_en.htm
#pragma once
#ifndef ITIME_H_INCLUDED
    #define ITIME_H_INCLUDED
    #include <time.h> 

    // typedef struct
    // {
    //     char readable_date[30];
    //     char date[15];
    //     char time[15];
    //     int hours;
    //     int minutes;
    //     int seconds;
    //     int dayOfWeek;
    //     unsigned long cur_timestamp;
    //     unsigned long raw_time;
    //     struct tm timeinfo;
    //     datetime_t *currDateTime;
    // } mytime_t;

    //void getInternetTime(mytime_t&);
    void getInternetTime();
#endif
