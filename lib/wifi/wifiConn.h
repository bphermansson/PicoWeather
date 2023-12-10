#pragma once
#ifndef WIFICONN_H_INCLUDED
#define WIFICONN_H_INCLUDED
    #include <Arduino.h>
    #include <WiFi.h>
    void wifi_connect(char *ipAddrPtr);
    //char * webserver_connect();
    String webserver_connect(char hostName[], int httpPort);
#endif