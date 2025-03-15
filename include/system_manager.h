#pragma once

#include <Arduino.h>
#include "WiFi.h"
#include "power_readings.h"
#include "PZEM004Tv30.h"

class SystemManager {
private:
    // Pin definitions
    static const int WIFI_LED_PIN = 2;
    static const int CHARGING_PIN = 35;  
    static const int RX_PIN = 16;
    static const int TX_PIN = 17;
    
    // Non-const static members
    static HardwareSerial pzemSerial;
    static PZEM004Tv30 pzem;
    static bool lastChargingState;

    // NTP related constants (removed constexpr)
    static const char* NTP_SERVER;
    static const long GMT_OFFSET_SEC;
    static const int DAYLIGHT_OFFSET_SEC;

public:
    static void setupPZEM();
    static bool setupWiFi();
    static bool isWiFiConnected();
    static bool reconnectWiFi();
    static bool syncTime();
    static void setupIndicators();
    static void updateWiFiLED(bool connected);
    static void updateChargingLED(bool charging);
    static PowerReadings getPowerReadings();
    static void checkAndUpdateChargingStatus();
    static bool isCharging();
    static PZEM004Tv30& getPZEM();
};
