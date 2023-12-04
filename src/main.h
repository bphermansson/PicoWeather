#pragma once
#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
    char ipAddr[] = "xxx.xxx.xxx.xxx";

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
#endif