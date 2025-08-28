# Bluetooth Hand Controller Setup Guide

## Prerequisites

1. **ESP32 with uploaded Arduino code** - Make sure your hand controller Arduino code is uploaded and running
2. **Bluetooth enabled** - Your computer should have Bluetooth capability

## Setup Steps

### 1. Upload Arduino Code
Make sure your Arduino code includes the Bluetooth initialization:
```cpp
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

void setup() {
  // Initialize Bluetooth with device name "HandController"
  SerialBT.begin("HandController");
  // ... rest of your setup code
}
```

### 2. Find and Pair Your Device

Run the automated setup script:
```bash
cd /home/sashad/Arduino/HandController/DataVis
./bluetooth_setup.sh
```

This script will:
- Start Bluetooth service
- Scan for devices
- Find your "HandController" device
- Attempt to pair and connect
- Show you the MAC address for manual connection

### 3. Start the Server

Once you have the MAC address of your hand controller, start the server:

**For Bluetooth connection:**
```bash
cd /home/sashad/Arduino/HandController/DataVis/build
./my_web_server bluetooth XX:XX:XX:XX:XX:XX
```

**For Serial connection (legacy):**
```bash
cd /home/sashad/Arduino/HandController/DataVis/build
./my_web_server serial /dev/ttyUSB0
```

### 4. Open the Web Interface

1. Open your web browser
2. Navigate to `http://localhost:8080`
3. The web interface should connect to the WebSocket server automatically
4. You should see finger values updating in real-time

## Troubleshooting

### Device Not Found
- Ensure your ESP32 is powered on
- Make sure the Arduino code is running (check Serial Monitor)
- Try restarting the ESP32
- Run `bluetoothctl scan on` manually and look for your device

### Connection Issues
- Try unpairing and re-pairing the device:
  ```bash
  bluetoothctl remove XX:XX:XX:XX:XX:XX
  bluetoothctl pair XX:XX:XX:XX:XX:XX
  ```
- Check if the device is already connected to another application
- Restart Bluetooth service: `sudo systemctl restart bluetooth`

### Server Issues
- Make sure no other application is using port 9002 (WebSocket) or 8080 (HTTP)
- Check that the WebSocket client connects successfully
- Look at server console output for error messages

## Manual Bluetooth Commands

If the automated script doesn't work, you can manually connect:

```bash
# Enable Bluetooth
bluetoothctl power on

# Scan for devices
bluetoothctl scan on

# Pair with device (replace XX:XX:XX:XX:XX:XX with actual MAC)
bluetoothctl pair XX:XX:XX:XX:XX:XX

# Connect to device
bluetoothctl connect XX:XX:XX:XX:XX:XX

# Start server
./my_web_server bluetooth XX:XX:XX:XX:XX:XX
```

## Data Format

The system sends data in the format: `a1234b5678c9012d3456e7890GGestureName`

- `a-e`: Raw finger sensor values (0-4095)
- `G`: Followed by gesture name (e.g., "Open", "Fist", "Point", etc.)

The server automatically reverses the finger values (4095 - original_value) to correct for inverted readings.
