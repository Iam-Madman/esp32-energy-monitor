#include "firebase_manager.h"
#include "credentials.h"
#include "debug_utils.h"

FirebaseData FirebaseManager::fbdo;
FirebaseAuth FirebaseManager::auth;
FirebaseConfig FirebaseManager::config;

const char* FirebaseManager::DEVICE_STATUS_PATH = "/deviceStatus";

bool FirebaseManager::setup() {
    DEBUG_PRINTLN("Initializing Firebase...");
    DEBUG_PRINTLN("Initializing Firebase connection...");
    config.api_key = FIREBASE_API_KEY;
    config.database_url = FIREBASE_DATABASE_URL;
    auth.user.email = FIREBASE_USER_EMAIL;
    auth.user.password = FIREBASE_USER_PASSWORD;
    
    Firebase.reconnectWiFi(true);
    config.token_status_callback = tokenStatusCallback;
    Firebase.begin(&config, &auth);
    
    DEBUG_PRINT("Waiting for Firebase authentication");
    unsigned long timeout = millis();
    while (auth.token.uid.empty() && millis() - timeout < 10000) {
        DEBUG_PRINT(".");
        delay(1000);
    }
    
    if (!auth.token.uid.empty()) {
        DEBUG_PRINTLN("\nFirebase authenticated successfully!");
        syncTime();  // Sync time after successful Firebase connection
        setupHeartbeat();  // Add this line
        DEBUG_PRINTLN("Firebase setup complete");
        return true;
    }
    DEBUG_PRINTLN("\nFirebase authentication failed!");
    return false;
}

void FirebaseManager::tokenStatusCallback(TokenInfo info) {
    // Print token status without accessing undefined members
    DEBUG_PRINT("Token Status: ");
    switch (info.status) {
        case token_status_ready:
            DEBUG_PRINTLN("Token is ready");
            break;
        case token_status_error:
            DEBUG_PRINTLN("Token error");
            break;
        case token_status_uninitialized:
            DEBUG_PRINTLN("Token is not initialized");
            break;
        default:
            DEBUG_PRINTLN("Unknown token status");
            break;
    }
}

void FirebaseManager::syncTime() {
    DEBUG_PRINT("Synchronizing time with NTP...");
    
    // Set the correct timezone for India (IST, UTC+5:30)
    setenv("TZ", "IST-5:30", 1);  
    tzset();

    // Sync time from NTP server
    configTime(0, 0, "pool.ntp.org");

    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        DEBUG_PRINTF("\nTime synchronized: %02d:%02d:%02d\n",
                      timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    } else {
        DEBUG_PRINTLN("\nFailed to sync time!");
    }
}

bool FirebaseManager::updateReadings(const PowerReadings& readings) {
    DEBUG_PRINTLN("Updating Firebase readings...");
    // Remove the isValid check to allow zero values
    // if (!readings.isValid) return false;
    
    // Only validate for NaN, not zero values
    if (isnan(readings.voltage) || isnan(readings.current) || 
        isnan(readings.power) || isnan(readings.frequency) ||
        isnan(readings.powerFactor) || isnan(readings.thd) ||
        isnan(readings.powerQuality)) {
        DEBUG_PRINTLN("Error: Invalid readings detected (NaN values)");
        return false;
    }

    DEBUG_PRINTLN("\n=== Updating Readings ===");
    DEBUG_PRINTF("Voltage: %.2fV\n", readings.voltage);
    DEBUG_PRINTF("Current: %.2fA\n", readings.current);
    DEBUG_PRINTF("Power: %.2fW\n", readings.power);
    DEBUG_PRINTF("Energy: %.2fWh\n", readings.energy);
    DEBUG_PRINTF("Frequency: %.2fHz\n", readings.frequency);
    DEBUG_PRINTF("Power Factor: %.2f\n", readings.powerFactor);
    DEBUG_PRINTF("Apparent Power: %.2fVA\n", readings.apparentPower);
    DEBUG_PRINTF("Reactive Power: %.2fVAR\n", readings.reactivePower);
    DEBUG_PRINTF("THD: %.2f%%\n", readings.thd);
    DEBUG_PRINTF("Power Quality: %.2f\n", readings.powerQuality);

    FirebaseJson jsonData;
    
    // Get current time
    time_t now;
    time(&now);
    jsonData.set("timestamp", (int)now);  // Unix timestamp in seconds

    // Add isCharging status along with other readings
    jsonData.set("isCharging", readings.isCharging);

    // Always send values, even if they're zero
    jsonData.set("voltage", readings.voltage);
    jsonData.set("current", readings.current);
    jsonData.set("power", readings.power);
    jsonData.set("energy", PowerReadings::accumulatedEnergy);  // Use the static member
    jsonData.set("frequency", readings.frequency);
    jsonData.set("powerFactor", readings.powerFactor);
    jsonData.set("apparentPower", readings.apparentPower);
    jsonData.set("reactivePower", readings.reactivePower);
    jsonData.set("loadImpedance", readings.loadImpedance);
    jsonData.set("distortionPower", readings.distortionPower);
    jsonData.set("thd", readings.thd);
    jsonData.set("powerQuality", readings.powerQuality);
    
    bool success = Firebase.RTDB.setJSON(&fbdo, "/readings", &jsonData);
    if (success) {
        DEBUG_PRINTLN("Readings updated successfully");
    } else {
        DEBUG_PRINTF("Failed to update readings: %s\n", fbdo.errorReason().c_str());
    }
    DEBUG_PRINTLN("Firebase update complete");
    return success;
}

bool FirebaseManager::updateBattery(uint8_t level) {
    DEBUG_PRINTLN("Updating Firebase battery level...");
    DEBUG_PRINTF("Updating battery level: %d%%\n", level);
    bool success = Firebase.RTDB.setInt(&fbdo, "battery", level);
    if (!success) {
        DEBUG_PRINTF("Failed to update battery: %s\n", fbdo.errorReason().c_str());
    }
    DEBUG_PRINTLN("Firebase battery update complete");
    return success;
}

bool FirebaseManager::checkResetFlag() {
    DEBUG_PRINTLN("Checking Firebase reset flag...");
    bool resetEnergy = false;
    if (Firebase.RTDB.getBool(&fbdo, "reset", &resetEnergy)) {
        if (resetEnergy) {
            DEBUG_PRINTLN("Reset flag detected!");
            // Return true so main loop can reset PZEM first
            DEBUG_PRINTLN("Firebase reset flag detected");
            return true;
            // Don't set reset flag to false here - it will be done after PZEM reset
        }
    } else {
        DEBUG_PRINTF("Failed to check reset flag: %s\n", fbdo.errorReason().c_str());
    }
    DEBUG_PRINTLN("Firebase reset flag check complete");
    return false;
}

// Add new method to clear reset flag
bool FirebaseManager::clearResetFlag() {
    DEBUG_PRINTLN("Clearing Firebase reset flag...");
    DEBUG_PRINTLN("Clearing reset flag...");
    bool success = Firebase.RTDB.setBool(&fbdo, "reset", false);
    if (success) {
        DEBUG_PRINTLN("Reset flag cleared successfully");
    } else {
        DEBUG_PRINTF("Failed to clear reset flag: %s\n", fbdo.errorReason().c_str());
    }
    DEBUG_PRINTLN("Firebase reset flag clear complete");
    return success;
}

void FirebaseManager::setupHeartbeat() {
    DEBUG_PRINTLN("Setting up Firebase heartbeat...");
    // Only set initial lastSeen timestamp
    FirebaseJson statusData;
    statusData.set("lastSeen", (int)time(nullptr));
    
    // Set initial timestamp
    if (Firebase.RTDB.setJSON(&fbdo, DEVICE_STATUS_PATH, &statusData)) {
        DEBUG_PRINTLN("Initial heartbeat timestamp set successfully");
    } else {
        DEBUG_PRINTF("Failed to set initial heartbeat: %s\n", fbdo.errorReason().c_str());
    }
    
    // No need for onDisconnect handler since we're using timestamp-based status
    DEBUG_PRINTLN("Firebase heartbeat setup complete");
}

void FirebaseManager::updateHeartbeat() {
    DEBUG_PRINTLN("Updating Firebase heartbeat...");
    FirebaseJson statusData;
    statusData.set("lastSeen", (int)time(nullptr));
    Firebase.RTDB.setJSON(&fbdo, "deviceStatus", &statusData);
    DEBUG_PRINTLN("Firebase heartbeat update complete");
}

bool FirebaseManager::loadSavedEnergy() {
    DEBUG_PRINTLN("Loading saved energy from Firebase...");
    if (Firebase.RTDB.getFloat(&fbdo, "readings/energy")) {
        float savedEnergy = fbdo.floatData();
        if (savedEnergy >= 0) {
            PowerReadings::accumulatedEnergy = savedEnergy;
            DEBUG_PRINTF("Loaded saved energy: %.2f Wh\n", savedEnergy);
            DEBUG_PRINTLN("Firebase saved energy load complete");
            return true;
        }
    }
    DEBUG_PRINTLN("No saved energy found, starting from 0");
    DEBUG_PRINTLN("Firebase saved energy load complete");
    return false;
}
