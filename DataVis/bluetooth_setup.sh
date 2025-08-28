#!/bin/bash

echo "Bluetooth Hand Controller Setup Script"
echo "======================================"

# Check if bluetooth service is running
echo "Checking Bluetooth service status..."
if ! systemctl is-active --quiet bluetooth; then
    echo "Starting Bluetooth service..."
    sudo systemctl start bluetooth
fi

# Enable Bluetooth adapter
echo "Enabling Bluetooth adapter..."
bluetoothctl power on

# Scan for devices
echo "Scanning for Bluetooth devices (looking for 'HandController')..."
echo "Please make sure your ESP32 is powered on and in pairing mode."
echo "Scanning for 10 seconds..."

bluetoothctl scan on &
SCAN_PID=$!
sleep 10
kill $SCAN_PID 2>/dev/null

echo ""
echo "Available Bluetooth devices:"
bluetoothctl devices

echo ""
echo "Looking for HandController device..."
HAND_CONTROLLER=$(bluetoothctl devices | grep -i "HandController" | head -1)

if [ -n "$HAND_CONTROLLER" ]; then
    MAC_ADDRESS=$(echo $HAND_CONTROLLER | cut -d' ' -f2)
    echo "Found HandController device: $MAC_ADDRESS"
    
    echo "Attempting to pair with device..."
    bluetoothctl pair $MAC_ADDRESS
    
    echo "Attempting to connect to device..."
    bluetoothctl connect $MAC_ADDRESS
    
    echo ""
    echo "To start the server with Bluetooth connection, run:"
    echo "./my_web_server bluetooth $MAC_ADDRESS"
    echo ""
    echo "MAC Address for manual connection: $MAC_ADDRESS"
else
    echo "HandController device not found. Please ensure:"
    echo "1. Your ESP32 is powered on"
    echo "2. The Arduino code is uploaded and running"
    echo "3. The device is in pairing mode"
    echo ""
    echo "You can manually connect by finding your device MAC address and running:"
    echo "./my_web_server bluetooth XX:XX:XX:XX:XX:XX"
fi
