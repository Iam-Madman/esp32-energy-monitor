# Mobile App Guide

## Installation

1. Download the latest APK:
   - [Download APK v1.0.0](https://github.com/Iam-Madman/esp32-energy-monitor/releases/download/v1.0.0/energy_monitor_esp32.apk)
   - [Project Website](https://energy-monitor-esp32.web.app/)

## Initial Setup

### Device Configuration
1. Configure WiFi credentials:
   - Edit credentials.h file
   - Enter correct SSID and password
   - Save and upload firmware
2. Verify connection:
   - Check WiFi icon in app
   - Monitor signal strength
   - Ensure stable internet

## Features Overview

### Authentication
- Secure email/password login
- Persistent authentication state
- Automatic session management
- Secure Firebase integration

### Dashboard Interface
- Real-time power metrics display
- Status indicators:
  - WiFi connection status with signal strength
  - Battery level with charging animation
  - Last update timestamp
  - Connection quality indicator
  
### Metrics Monitoring

#### Basic Metrics
- Voltage (V)
- Current (A)
- Power (W)
- Frequency (Hz)

#### Advanced Metrics
- Apparent Power (VA)
- Power Factor
- Reactive Power (VAR)
- Load Impedance (Ω)
- THD (%)
- Distortion Power (VA)
- Power Quality

### Battery Management
- Percentage display
- Charging status indicator
- Low battery warnings:
  - Critical alert at 5%
  - Warning at 10%
- Dynamic battery icon updates

### Data Management
- Real-time Firebase synchronization
- Automatic data refresh
- Energy counter reset functionality
- Timestamp tracking

### User Interface Features
- Dark theme optimization
- Animated metric cards
- Interactive card explanations
- Responsive layout design
- Pull-to-refresh functionality

## Navigation Guide

### Main Dashboard
1. **Top Bar**
   - Menu access
   - Device status
   - Battery indicator
   - Reset button

2. **Energy Display**
   - Total energy consumption
   - Interactive information card
   - Reset instructions

3. **Metrics Section**
   - Basic metrics cards
   - Advanced metrics cards
   - Last update timestamp

### Menu Options
- About section
- Logout functionality
- Device information

## Troubleshooting

### Connection Issues
- Verify WiFi connection
- Check Firebase connectivity
- Ensure device is powered on
- Try app restart

### Data Updates
- Pull down to refresh
- Check last update timestamp
- Verify device status indicator
- Reset energy counter if needed

### Battery Issues
- Verify charging connection
- Monitor battery percentage
- Observe charging indicator
- Address low battery warnings

## Technical Details

### Metric Descriptions

```
Voltage: The potential difference (V)
Current: Rate of electric charge flow (A)
Power: Real power consumption (W)
Energy: Total power over time (Wh)
Frequency: AC cycles per second (Hz)
Apparent Power: Total power supplied (VA)
Power Factor: Power efficiency ratio
Reactive Power: Stored/released power (VAR)
Load Impedance: Circuit opposition (Ω)
THD: Harmonic distortion measure (%)
Distortion Power: Harmonic power loss (VA)
Power Quality: Overall supply quality
```

### Battery Indicators
- Full: ≥90%
- High: ≥80%
- Good: ≥60%
- Medium: ≥50%
- Low: ≥40%
- Critical: ≥20%
- Alert: <20%
