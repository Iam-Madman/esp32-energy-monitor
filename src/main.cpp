#include <Arduino.h>
#include "system_manager.h"
#include "battery_monitor.h"
#include "firebase_manager.h"
#include "power_readings.h"
#include "debug_utils.h"

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
FirebaseData fbdo;

const unsigned long UPDATE_INTERVAL = 2000;  // 2 seconds in milliseconds

void setup() {
    Serial.begin(9600);
    DEBUG_PRINTLN("\n=== ESP32 Energy Monitor Starting (Debug Mode) ===\n");
    
    SystemManager::setupPZEM();
    SystemManager::setupIndicators();
    
    // Wait indefinitely for WiFi connection
    while (!SystemManager::setupWiFi()) {
        DEBUG_PRINTLN("WiFi setup failed! Retrying...");
        delay(1000);
        ESP.restart();
    }
    
    if (!SystemManager::syncTime()) {
        DEBUG_PRINTLN("Time sync failed! System halted.");
        ESP.restart();
        return;
    }
    
    if (!FirebaseManager::setup()) {
        DEBUG_PRINTLN("Firebase setup failed! System halted.");
        ESP.restart();
        return;
    }
    
    // Load saved energy after Firebase setup
    FirebaseManager::loadSavedEnergy();
    
    BatteryMonitor::setup();
    DEBUG_PRINTLN("\nSystem initialization complete!");
    DEBUG_PRINTLN("=== PZEM-004T v3 Monitor Ready ===\n");
    signupOK = true;
    DEBUG_PRINTLN("Setup complete!");
}

void loop() {
    static unsigned long lastUpdateTime = 0;
    static unsigned long lastDebugTime = 0;
    unsigned long currentTime = millis();

    // Always get readings regardless of WiFi status
    if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
        PowerReadings readings = SystemManager::getPowerReadings();
        lastUpdateTime = currentTime;

        // Check WiFi status
        if (!SystemManager::isWiFiConnected()) {
            SystemManager::updateWiFiLED(false);
            SystemManager::reconnectWiFi(); // Non-blocking now
        } else {
            SystemManager::updateWiFiLED(true);
            
            if (Firebase.ready() && signupOK) {
                uint8_t batteryLevel = BatteryMonitor::getBatteryPercentage();
                FirebaseManager::updateBattery(batteryLevel);
                
                if (FirebaseManager::checkResetFlag()) {
                    PowerReadings::resetEnergy();
                    FirebaseManager::clearResetFlag();
                }
                
                FirebaseManager::updateReadings(readings);
                FirebaseManager::updateHeartbeat();
            }
        }
    }

    if (currentTime - lastDebugTime >= 5000) {  // Debug output every 5 seconds
        DEBUG_PRINTF("System uptime: %lu ms\n", currentTime);
        DEBUG_PRINTF("WiFi Status: %s\n", SystemManager::isWiFiConnected() ? "Connected" : "Disconnected");
        lastDebugTime = currentTime;
    }
}

