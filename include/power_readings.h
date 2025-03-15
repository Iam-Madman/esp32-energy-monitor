#pragma once
#include <Arduino.h>

struct PowerReadings {
    float voltage = 0;
    float current = 0;
    float power = 0;
    float energy = 0;
    float frequency = 0;
    float powerFactor = 0;
    float apparentPower = 0;
    float reactivePower = 0;
    float loadImpedance = 0;
    float distortionPower = 0;
    float thd = 0;
    float powerQuality = 0;
    bool isValid = false;
    bool isCharging = false;  // Add this line for charging status
    
    static float accumulatedEnergy;  // Static member to keep track of total energy
    static unsigned long lastMeasurementTime;  // To track time between measurements
    
    static void resetEnergy() {
        accumulatedEnergy = 0;
        lastMeasurementTime = millis();
    }
};
