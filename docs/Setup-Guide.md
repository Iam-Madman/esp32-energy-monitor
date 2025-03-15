# Setup Guide

## Development Environment Setup

1. Install PlatformIO IDE
   ```bash
   # VSCode Extension
   code --install-extension platformio.platformio-ide
   ```

2. Clone Repository
   ```bash
   git clone https://github.com/Iam-Madman/esp32-energy-monitor.git
   cd esp32-energy-monitor
   ```

3. Configure Credentials
   ```bash
   cp include/credentials.h.template include/credentials.h
   # Edit credentials.h with your WiFi and Firebase details
   ```

## Hardware Setup

1. Power Circuit
   - Connect battery to TP4056
   - Wire boost converter
   - Connect voltage divider

2. PZEM Module Setup
   - Wire UART connections
   - Connect power lines
   - Mount current transformers

## Software Configuration

1. Firebase Setup
   ```bash
   # Install Firebase CLI
   npm install -g firebase-tools
   
   # Login to Firebase
   firebase login
   
   # Initialize project
   firebase init
   ```

2. Mobile App Setup
   ```bash
   cd app/energy_monitor
   flutter pub get
   flutter run
   ```

## First-Time Operation

1. Power up the device
2. Connect to WiFi
3. Verify sensor readings
4. Test mobile app connection

## Verification Steps

- [ ] Battery charging works
- [ ] WiFi connects successfully
- [ ] PZEM sensors read correctly
- [ ] Data uploads to Firebase
- [ ] Mobile app receives data
