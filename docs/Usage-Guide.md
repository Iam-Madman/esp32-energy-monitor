# Usage Guide

## Initial Setup

### Firebase Configuration
1. Create a Firebase project at [Firebase Console](https://console.firebase.google.com/)
2. Edit credentials.h with your Firebase details:
   ```cpp
   // Replace with your Firebase project details
   #define FIREBASE_API_KEY "Your-Firebase-API-Key"
   #define FIREBASE_DATABASE_URL "Your-Firebase-Database-URL"
   #define FIREBASE_USER_EMAIL "Your-Auth-Email"
   #define FIREBASE_USER_PASSWORD "Your-Auth-Password"
   ```
3. Important:
   - Never share your API keys
   - Use a strong password
   - Create a dedicated Firebase user
   - Enable Email/Password authentication in Firebase Console

### WiFi Configuration
1. Copy credentials template:
   ```bash
   cp include/credentials.h.template include/credentials.h
   ```
2. Edit credentials.h with your WiFi details:
   ```cpp
   #define WIFI_SSID "Your_WiFi_Name"
   #define WIFI_PASS "Your_WiFi_Password"
   ```
3. Ensure strong WiFi signal where device is placed
4. Device requires stable internet for Firebase connectivity

## Basic Operation

### Power On
1. Check battery level in mobile app
2. Verify WiFi connection (Blue LED indicates connection)
3. Monitor charging status in app (No LED indicator)
4. Wait for initialization (~5 seconds)

### Connection Setup
1. Connect power inputs:
   - 16A input: Use the switch-controlled socket
   - 30A input: Use binding posts
2. Verify readings on mobile app
3. Check real-time data flow

### Connection Requirements
- Place device where WiFi signal is strong
- Minimum WiFi signal strength: -70dBm
- Stable internet connection required
- Check WiFi status in mobile app

## Monitoring Functions

### Power Readings
- Voltage: 110V-240V AC
- Current: Up to 16A/30A
- Power: Active/Reactive
- Energy: Cumulative usage
- Frequency: Grid stability
- Power Factor: System efficiency

### Battery Management
- Charging status shown in mobile app
- Voltage monitoring (GPIO34)
- Low battery alerts in app
- Real-time battery percentage

### Data Collection
- Real-time measurements
- 5-second update intervals
- Automatic cloud sync
- Local data storage

## Safety Guidelines

### Operation Safety
- Keep device dry
- Ensure proper ventilation
- Avoid overloading
- Monitor temperature

### Maintenance
1. Regular checks:
   - Connection security
   - LED indicators
   - Sensor accuracy
   - Battery health

2. Periodic tasks:
   - Clean connections
   - Update firmware
   - Verify calibration
   - Check mounting

## Best Practices

### Optimal Usage
- Place in ventilated area
- Monitor regularly
- Keep connections tight
- Update as prompted

### Data Management
- Regular backups
- Export monthly data
- Review trends
- Track anomalies

### Power Saving
- Use sleep mode when idle
- Optimize update frequency
- Monitor battery health
- Manage WiFi connection
