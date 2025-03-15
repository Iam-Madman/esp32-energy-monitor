# Hardware Guide

## Components List

### Core Components
- ESP32 Development Board
- PZEM-004T v3.0 Module (2x)
- 18650 Li-ion Battery (3.6V 2600mAh)
- TP4056 Battery Charging Module
- MT3608 Boost Converter Module

### Additional Components
- Level Shifters (3.3V to 5V)
- Resistors for Voltage Divider
- PCB or Prototype Board
- Case and Mounting Hardware
- Power Connectors
- Status LEDs

## Circuit Diagram

```
                    ┌──────────────┐
                    │   ESP32      │
                    │              │
   ┌────────┐      │         TX2  ├───────┐
   │TP4056  │      │         RX2  ├───────┤
   │        ├──────┤         GPIO34├───────┤
   └────────┘      │              │       │
                    └──────────────┘       │
                                          │
   ┌────────┐                    ┌────────┴───┐
   │Battery ├────────────────────┤PZEM-004T #1│
   └────────┘                    └────────────┘
                                     │
                                ┌────┴───────┐
                                │PZEM-004T #2│
                                └────────────┘
```

## Pin Configuration

| ESP32 Pin | Connection              |
|-----------|------------------------|
| TX2 (GPIO17) | PZEM RX            |
| RX2 (GPIO16) | PZEM TX            |
| GPIO34      | Battery Voltage ADC  |
| GPIO35      | Charging Status      |
| GPIO2       | Status LED           |
| 3.3V        | Level Shifter LV     |
| 5V          | Level Shifter HV     |

## Assembly Instructions

1. Power Circuit
   - Connect TP4056 to battery
   - Wire boost converter output
   - Connect TP4056 CHRG pin to GPIO35
   - Add voltage divider for monitoring

2. Communication Circuit
   - Install level shifters
   - Wire UART connections
   - Connect status LEDs

3. Sensor Integration
   - Mount PZEM modules
   - Connect voltage lines
   - Install current transformers

4. Final Assembly
   - Mount components in case
   - Label connections
   - Perform safety checks

## Safety Considerations

⚠️ **Important Safety Notes:**
- Use appropriate wire gauge
- Ensure proper insulation
- Follow electrical codes
- Test before deployment
