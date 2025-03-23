# ESP32 Based Energy Monitoring System

A comprehensive portable energy monitoring solution using ESP32 and PZEM-004T v3.0 sensors.

## Quick Links

🌐 [Project Website](https://energy-monitor-esp32.web.app/)  
📱 [Download App](https://github.com/Iam-Madman/esp32-energy-monitor/releases/download/v1.0.0/energy_monitor_esp32.apk)  
📖 [Documentation](https://github.com/Iam-Madman/esp32-energy-monitor/wiki)  
💻 [Source Code](https://github.com/Iam-Madman/esp32-energy-monitor)

## Web Interface

Visit our [project website](https://energy-monitor-esp32.web.app/) for:
- Interactive demonstrations
- Visual setup guides
- Hardware assembly instructions
- Live data visualization examples
- Mobile app download
- Comprehensive documentation

## Features

- Dual input power monitoring (16A and 30A)
- Real-time measurements:
  - Voltage
  - Current
  - Power
  - Energy
  - Frequency
  - Power Factor
- Battery-powered operation with status monitoring
- WiFi connectivity
- Web interface for real-time monitoring
- Mobile app support
- Data logging capabilities

## Hardware Components

- ESP32 Development Board
- PZEM-004T v3.0 Module
- 3.6V 2600mAh Li-ion Battery (18650)
- TP4056 Battery Charging Module
- Boost Converter Module
- Level Shifters
- Voltage Divider for Battery Monitoring
- Power Management Circuit

### Input Configuration
- 16A input: Switch controlled with 3-pin cable
- 30A input: Direct binding posts
- Common Features:
  - 4mm² wire rating
  - CT based current measurement
  - Phase-Neutral voltage measurement

## Setup Instructions

1. Install PlatformIO IDE
2. Clone this repository
3. Configure credentials:
   ```bash
   cp include/credentials.h.template include/credentials.h
   ```
4. Update credentials.h with your settings

## Pin Configuration

| ESP32 Pin | Connection |
|-----------|------------|
| TX2       | PZEM RX   |
| RX2       | PZEM TX   |
| 3.3V      | PZEM VCC  |
| GND       | PZEM GND  |
| GPIO34    | Battery Voltage Divider (ADC) |
| GPIO35    | Battery Charging Status |

> Note: GPIO34 (ADC1_CH6) monitors battery voltage through a voltage divider circuit (3.3V-4.2V to 0-3.3V range).
> GPIO35 connects to the TP4056 CHRG pin to monitor charging status.

![Untitled Diagram drawio (1)](https://github.com/user-attachments/assets/713d54ae-8ab1-43db-aa85-920dc5effde7)


## Security Notes

- The `credentials.h` file is git-ignored to protect sensitive data
- Use `credentials.h.template` as reference
- Secure your API keys and credentials
- Consider using environment variables for CI/CD

## Mobile Application

A Flutter-based companion app is available in the `/app/energy_monitor/` directory, offering:
- Real-time monitoring
- Historical data analysis
- Battery status tracking
- Multiple measurement modes
- Theme customization
- Data export features

## License


This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## Developer

**Abinav Bino**
- LinkedIn: [abinav-bino](https://www.linkedin.com/in/abinav-bino/)
- GitHub: [Iam-Madman](https://github.com/Iam-Madman)

Contributions and feedback are welcome! Feel free to open issues or submit pull requests.
