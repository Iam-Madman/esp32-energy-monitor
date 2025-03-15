# Energy Monitor

A Flutter application for monitoring energy consumption using ESP32 and PZEM-004T module.

## Supported Platforms

- Android

## Getting Started

1. Make sure you have Flutter installed and set up
2. Clone the repository
3. Install FlutterFire CLI:
   ```bash
   dart pub global activate flutterfire_cli
   ```
4. Run `flutter pub get` to install dependencies
5. Configure Firebase:
   ```bash
   # Install Firebase CLI if not already installed
   npm install -g firebase-tools
   
   # Login to Firebase
   firebase login
   
   # Configure FlutterFire
   flutterfire configure --project=your-project-id
   ```
6. Run the application:
   ```bash
   flutter run -d android
   ```

## Features

### Real-time Monitoring
- Live voltage, current, and power readings
- Power factor monitoring
- Energy consumption tracking
- Battery status monitoring

### Data Analysis
- Historical data visualization
- Usage patterns and trends
- Export capabilities for detailed analysis
- Custom time period selection

### User Interface
- Intuitive dashboard design
- Dark/Light theme support
- Responsive layout for various screen sizes
- Real-time graph visualization

### System Integration
- Firebase real-time database integration
- Automatic data synchronization
- Offline capability
- Multi-device support

## Technical Requirements

- Flutter 3.0.0 or higher
- Dart 2.17.0 or higher
- Firebase account and project setup
- Android Studio for deployment

## Setup Instructions

1. Install Flutter SDK
2. Configure Firebase:
   ```bash
   flutterfire configure
   ```
3. Install dependencies:
   ```bash
   flutter pub get
   ```
4. Run the application:
   ```bash
   flutter run
   ```

## Building for Production

### Android
```bash
flutter build apk --release
```

## Developer

**Abinav Bino**
- LinkedIn: [abinav-bino](https://www.linkedin.com/in/abinav-bino/)
- GitHub: [Iam-Madman](https://github.com/Iam-Madman)

For detailed system architecture and implementation details, please refer to the main project README.
