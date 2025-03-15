#pragma once

#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include "power_readings.h"

class FirebaseManager {
private:
    static FirebaseData fbdo;
    static FirebaseAuth auth;
    static FirebaseConfig config;
    static const char* DEVICE_STATUS_PATH;
    static void tokenStatusCallback(TokenInfo info);
    static void syncTime();
    static void setupHeartbeat();

public:
    static bool setup();
    static bool updateReadings(const PowerReadings& readings);
    static bool updateBattery(uint8_t level);
    static bool checkResetFlag();
    static bool clearResetFlag();
    static void updateHeartbeat();
    static bool updateChargingStatus(bool isCharging);
    static bool loadSavedEnergy();
};
