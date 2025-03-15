#pragma once

#include <Arduino.h>

class BatteryMonitor {
public:
    static void setup();
    static uint8_t getBatteryPercentage();
    static float getBatteryVoltage();

private:
    static const uint8_t BATTERY_PIN = 34;  // GPIO34 (ADC1_6)
    static const float R1;  // First resistor (kΩ)
    static const float R2;  // Second resistor (kΩ)
    static const float VOLTAGE_SCALER;  // Voltage divider factor
    static const int SAMPLE_COUNT = 20;  // Number of ADC samples to average
};
