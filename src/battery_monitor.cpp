#include "battery_monitor.h"
#include "credentials.h"
#include "debug_utils.h"
#include <esp_adc_cal.h>

// Define static member constants
const float BatteryMonitor::R1 = 96.5f;  // First resistor (kΩ)
const float BatteryMonitor::R2 = 45.5f;  // Second resistor (kΩ)
const float BatteryMonitor::VOLTAGE_SCALER = (BatteryMonitor::R1 + BatteryMonitor::R2) / BatteryMonitor::R2;

static esp_adc_cal_characteristics_t adc_chars;

// Battery discharge curve mapping
// Modified to include an extra anchor for smoother values below 10%
const float BATTERY_LEVELS[][2] = {
    {4.20, 100},  // Full charge
    {4.00, 80},
    {3.80, 60},
    {3.60, 40},
    {3.40, 20},
    {3.30, 10},   // New anchor point for 10%
    {3.00, 5},
    {2.70, 0}     // Cutoff at 0%
};

void BatteryMonitor::setup() {
    DEBUG_PRINTLN("Initializing Battery Monitor...");
    analogReadResolution(12);         // Set ADC resolution to 12 bits (0-4095)
    analogSetAttenuation(ADC_11db);     // Use 11dB attenuation for full range (0-3.3V)
    
    // ESP32 ADC Calibration
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 1100, &adc_chars);
    DEBUG_PRINTLN("Battery Monitor setup complete");
}

float BatteryMonitor::getBatteryVoltage() {
    static uint32_t lastSampleTime = 0;
    static uint32_t sampleCount = 0;
    static uint32_t adcSum = 0;
    static const unsigned long SAMPLE_INTERVAL = 1; // 1ms between samples
    
    unsigned long currentTime = millis();
    
    // Check if it's time for a new sample
    if (currentTime - lastSampleTime >= SAMPLE_INTERVAL) {
        lastSampleTime = currentTime;
        adcSum += analogRead(BATTERY_PIN);
        sampleCount++;
        
        // If we have all samples, calculate the average
        if (sampleCount >= SAMPLE_COUNT) {
            uint32_t adcAverage = adcSum / SAMPLE_COUNT;
            float vOut = esp_adc_cal_raw_to_voltage(adcAverage, &adc_chars) / 1000.0;
            float vBat = vOut * VOLTAGE_SCALER;
            
            // Reset for next batch
            adcSum = 0;
            sampleCount = 0;
            
            DEBUG_PRINTF("ADC Average: %d, Vout: %.3fV, VBat: %.3fV\n", 
                         adcAverage, vOut, vBat);
            return vBat;
        }
    }
    
    // Return last valid reading if sampling is in progress
    float vOut = esp_adc_cal_raw_to_voltage(adcSum / (sampleCount ? sampleCount : 1), &adc_chars) / 1000.0;
    return vOut * VOLTAGE_SCALER;
}

uint8_t BatteryMonitor::getBatteryPercentage() {
    float vBat = getBatteryVoltage();
    int tableSize = sizeof(BATTERY_LEVELS) / sizeof(BATTERY_LEVELS[0]);

    // Return max/min if out of bounds
    if (vBat >= BATTERY_LEVELS[0][0]) return BATTERY_LEVELS[0][1];
    if (vBat <= BATTERY_LEVELS[tableSize - 1][0]) return BATTERY_LEVELS[tableSize - 1][1];
    
    // Find the segment where vBat lies by checking adjacent anchors.
    for (int i = 0; i < tableSize - 1; i++) {
        if (vBat <= BATTERY_LEVELS[i][0] && vBat >= BATTERY_LEVELS[i+1][0]) {
            float v1 = BATTERY_LEVELS[i][0];
            float v2 = BATTERY_LEVELS[i+1][0];
            float p1 = BATTERY_LEVELS[i][1];
            float p2 = BATTERY_LEVELS[i+1][1];
            // Linear interpolation
            float perc = p1 + ((vBat - v1) / (v2 - v1)) * (p2 - p1);
            uint8_t percentage = (uint8_t)perc;
            DEBUG_PRINTF("Battery: %.2fV (%d%%)\n", vBat, percentage);
            return percentage;
        }
    }
    
    return 0;
}
