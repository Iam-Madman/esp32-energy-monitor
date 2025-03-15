#include "system_manager.h"
#include "credentials.h"
#include "firebase_manager.h"
#include "debug_utils.h"
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Initialize static members
HardwareSerial SystemManager::pzemSerial(1);  // Serial1
PZEM004Tv30 SystemManager::pzem(SystemManager::pzemSerial, RX_PIN, TX_PIN);  // Use class constants
const char* SystemManager::NTP_SERVER = "pool.ntp.org";
const long SystemManager::GMT_OFFSET_SEC = 19800;     // GMT+5:30
const int SystemManager::DAYLIGHT_OFFSET_SEC = 0;
bool SystemManager::lastChargingState = false;

void SystemManager::setupPZEM() {
    DEBUG_PRINTLN("Setting up PZEM...");
    DEBUG_PRINTLN("=== PZEM Setup ===");
    SystemManager::pzemSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    delay(100);  // Brief delay for serial initialization
    DEBUG_PRINTLN("PZEM setup complete");
}

PowerReadings SystemManager::getPowerReadings() {
    DEBUG_PRINTLN("Getting power readings...");
    PowerReadings readings;
    unsigned long currentTime = millis();
    
    // Read charging status with voltage divider calculation - SINGLE SOURCE OF TRUTH
    float voltage = analogRead(CHARGING_PIN)/1000.0;  // Convert to volts
    readings.isCharging = voltage > 1.3f;
    
    DEBUG_PRINTF("⚡ Charging Status: %s (Voltage: %.2fV)\n", 
                 readings.isCharging ? "CHARGING" : "NOT CHARGING",
                 voltage);
    
    // Read values from PZEM
    readings.voltage = pzem.voltage();
    readings.current = pzem.current();
    readings.power = pzem.power();
    readings.frequency = pzem.frequency();
    readings.powerFactor = pzem.pf();
    
    // Check validity first
    readings.isValid = !isnan(readings.voltage) && !isnan(readings.current) && 
                      !isnan(readings.power);
    
    if (readings.isValid) {
        // Always calculate energy increment regardless of WiFi status
        if (PowerReadings::lastMeasurementTime > 0) {
            float hoursSinceLastMeasurement = (currentTime - PowerReadings::lastMeasurementTime) / 3600000.0;
            float energyIncrement = readings.power * hoursSinceLastMeasurement;
            PowerReadings::accumulatedEnergy += energyIncrement;
        }
        PowerReadings::lastMeasurementTime = currentTime;
        readings.energy = PowerReadings::accumulatedEnergy;
        
        // Debug output
        DEBUG_PRINTLN("\n********************************");
        DEBUG_PRINTF("🔌 Voltage:      %.2f V\n", readings.voltage);
        DEBUG_PRINTF("⚡ Current:      %.2f A\n", readings.current);
        DEBUG_PRINTF("💡 Power:        %.2f W\n", readings.power);
        DEBUG_PRINTF("🔋 Energy Used:  %.2f Wh\n", readings.energy);
        DEBUG_PRINTF("🔄 Frequency:    %.2f Hz\n", readings.frequency);
        DEBUG_PRINTF("✅ Power Factor: %.2f\n", readings.powerFactor);
        DEBUG_PRINTLN("********************************\n");
        
        // Calculate derived metrics with validation
        readings.apparentPower = readings.voltage * readings.current;
        float powerFactorSquared = readings.powerFactor * readings.powerFactor;
        if (powerFactorSquared <= 1.0) {  // Validate power factor
            readings.reactivePower = readings.apparentPower * sqrt(1 - powerFactorSquared);
        } else {
            readings.reactivePower = 0;
        }
        
        readings.loadImpedance = (readings.current > 0) ? readings.voltage / readings.current : 0;
        
        // Validate power calculations before computing distortion power
        if (readings.apparentPower >= readings.power && readings.power > 0) {
            float distPowerSquared = pow(readings.apparentPower, 2) - pow(readings.power, 2);
            if (distPowerSquared >= 0) {  // Ensure we don't sqrt a negative number
                readings.distortionPower = sqrt(distPowerSquared);
                readings.thd = (readings.distortionPower / readings.power) * 100;
            } else {
                readings.distortionPower = 0;
                readings.thd = 0;
            }
        } else {
            readings.distortionPower = 0;
            readings.thd = 0;
        }
        
        // Ensure power quality is a valid number
        if (readings.thd >= 0 && readings.thd <= 100) {
            readings.powerQuality = readings.powerFactor * (1 - readings.thd/100);
        } else {
            readings.powerQuality = readings.powerFactor;
        }

        // Final validation to ensure no NaN values
        if (isnan(readings.thd)) readings.thd = 0;
        if (isnan(readings.powerQuality)) readings.powerQuality = 0;

    } else {
        DEBUG_PRINTLN("⚠️ Error: No response from PZEM! Check wiring.");
        // Reset all readings to zero
        readings.voltage = 0;
        readings.current = 0;
        readings.power = 0;
        readings.frequency = 0;
        readings.powerFactor = 0;
        readings.apparentPower = 0;
        readings.reactivePower = 0;
        readings.loadImpedance = 0;
        readings.distortionPower = 0;
        readings.thd = 0;
        readings.powerQuality = 0;
        readings.isValid = false;

        // Keep the accumulated energy even when readings are invalid
        readings.energy = PowerReadings::accumulatedEnergy;

        DEBUG_PRINTF("⚡ Charging Status: %s (Voltage: %.2fV, PZEM failed)\n", 
                     readings.isCharging ? "CHARGING" : "NOT CHARGING",
                     voltage);
    }
    DEBUG_PRINTF("V:%.1fV, I:%.3fA, P:%.1fW, E:%.1fkWh\n", 
                 readings.voltage, readings.current, 
                 readings.power, readings.energy);
    return readings;
}

void SystemManager::setupIndicators() {
    pinMode(WIFI_LED_PIN, OUTPUT);
    pinMode(CHARGING_PIN, INPUT);
    analogSetPinAttenuation(CHARGING_PIN, ADC_11db);
    digitalWrite(WIFI_LED_PIN, LOW);  // Start with LED off (normal logic)
    DEBUG_PRINTLN("LED Indicators setup complete");
}

void SystemManager::updateWiFiLED(bool connected) {
    DEBUG_PRINTF("Setting WiFi LED: %s (Pin %d to %s)\n", 
                 connected ? "ON" : "OFF",
                 WIFI_LED_PIN,
                 connected ? "HIGH" : "LOW");
                 
    digitalWrite(WIFI_LED_PIN, connected ? HIGH : LOW);  // Normal logic: HIGH = ON, LOW = OFF
}

bool SystemManager::setupWiFi() {
    DEBUG_PRINTLN("Connecting to WiFi networks...");
    DEBUG_PRINTLN("\n=== WiFi Setup ===");
    WiFi.mode(WIFI_STA);
    
    for (int i = 0; i < NETWORK_COUNT; i++) {
        DEBUG_PRINTF("\nAttempting connection to: %s\n", WIFI_NETWORKS[i].ssid);
        WiFi.begin(WIFI_NETWORKS[i].ssid, WIFI_NETWORKS[i].password);
        
        unsigned long startAttemptTime = millis();
        
        while (WiFi.status() != WL_CONNECTED && 
               millis() - startAttemptTime < 10000) { // 10 second timeout per network
            DEBUG_PRINT(".");
            delay(500);
        }
        
        if (WiFi.status() == WL_CONNECTED) {
            DEBUG_PRINTLN("\nConnected successfully!");
            DEBUG_PRINTF("IP: %s\n", WiFi.localIP().toString().c_str());
            DEBUG_PRINTF("RSSI: %ddBm\n", WiFi.RSSI());
            DEBUG_PRINTLN("Turning ON WiFi LED");
            updateWiFiLED(true);  // Turn LED ON when connected
            DEBUG_PRINTF("WiFi connected. IP: %s\n", WiFi.localIP().toString().c_str());
            return true;
        }
        
        DEBUG_PRINTF("\nFailed to connect to %s\n", WIFI_NETWORKS[i].ssid);
        WiFi.disconnect(true);  // Disconnect before trying next network
        delay(1000);  // Brief delay before next attempt
    }
    DEBUG_PRINTLN("Turning OFF WiFi LED");
    updateWiFiLED(false);  // Make sure LED is OFF when not connected
    DEBUG_PRINTLN("WiFi connection failed");
    return false;
}

bool SystemManager::isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

bool SystemManager::reconnectWiFi() {
    static unsigned long lastReconnectAttempt = 0;
    const unsigned long RECONNECT_INTERVAL = 5000; // Try every 5 seconds
    
    if (isWiFiConnected()) return true;
    
    unsigned long currentMillis = millis();
    if (currentMillis - lastReconnectAttempt < RECONNECT_INTERVAL) {
        return false; // Too soon to try again
    }
    
    lastReconnectAttempt = currentMillis;
    DEBUG_PRINTLN("\nAttempting WiFi reconnection...");
    
    // Quick disconnect without delay
    WiFi.disconnect(true);
    
    // Single attempt to connect
    if (setupWiFi()) {
        DEBUG_PRINTLN("WiFi reconnected successfully!");
        return true;
    }
    
    DEBUG_PRINTLN("Reconnection attempt failed");
    return false;
}

bool SystemManager::syncTime() {
    configTime(SystemManager::GMT_OFFSET_SEC, 
              SystemManager::DAYLIGHT_OFFSET_SEC, 
              SystemManager::NTP_SERVER);
    
    int retries = 0;
    const int MAX_RETRIES = 5;
    time_t now;
    
    while (time(&now) < 1600000000 && retries < MAX_RETRIES) {
        DEBUG_PRINTLN("Waiting for NTP time sync...");
        delay(1000);
        retries++;
    }
    
    if (retries >= MAX_RETRIES) {
        DEBUG_PRINTLN("Failed to sync time!");
        return false;
    }
    
    DEBUG_PRINTLN("Time synchronized successfully");
    return true;
}

PZEM004Tv30& SystemManager::getPZEM() {
    return pzem;
}
