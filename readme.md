# ESP8266 Stairled Access Point Design Document

## 1. Overview

The ESP8266 Stairled Access Point is a dedicated device that provides WiFi connectivity for Stairled sensors. It offers a simple configuration interface and integrates with the main Stairled system (Raspberry Pi Zero W).

## 2. Hardware Components

- ESP8266 microcontroller
- Power supply (e.g., USB or dedicated power input)

## 3. Functionality Requirements

### 3.1 Access Point Operation
- Create a WiFi access point with a unique name (e.g., "stairled-ap-<uniqid>")
- Manage connected clients (Stairled sensors)
- Provide DHCP services to connected clients

### 3.2 Configuration
- Offer a simple web-based configuration interface
- Store configuration in non-volatile memory (e.g., EEPROM or SPIFFS)
- Provide API endpoints for remote configuration updates

### 3.3 Main Stairled System Integration
- Scan for and detect the main Stairled webserver (Raspberry Pi Zero W) on the network
- Redirect newly connected clients to the main Stairled webserver when available

### 3.4 OTA Updates
- Support Over-The-Air firmware updates via file upload

## 4. Configuration Options

- Access Point Name
- WiFi Password
- Admin Username and Password for configuration interface

## 5. User Interface

### 5.1 Configuration Web Interface
- Simple, responsive design
- Access point settings (name, password)
- Admin credentials management
- Connected clients list
- Firmware update interface with file upload

### 5.2 API Endpoints
- `/api/config` - GET/POST endpoint for configuration retrieval and updates
- `/api/clients` - GET endpoint to retrieve connected clients list
- `/api/update` - POST endpoint for firmware update file upload

## 6. Captive Portal

- Automatically redirect new clients to the configuration interface
- If main Stairled webserver is detected, redirect to its IP address instead

## 7. Security Considerations

- Use WPA2 encryption for the access point
- Implement secure storage for sensitive configuration data
- Rate-limit configuration attempts to prevent brute-force attacks

## 8. Failsafe and Recovery

- Implement watchdog timer to recover from crashes
- Store last known good configuration
- Provide a physical reset method (e.g., button press on boot) to restore default settings

## 9. Integration with Main Stairled System

- Periodically scan for the main Stairled webserver on the network
- Update internal state when main system is detected or lost
- Provide connection status to connected clients

## 10. OTA Update Process

### 10.1 Firmware Update
1. User accesses the configuration interface
2. Navigate to the firmware update section
3. Select and upload the new firmware file
4. ESP8266 verifies the file and applies the update
5. Device reboots with new firmware

## 11. Implementation Details

### 11.1 Core Libraries
- ESP8266WiFi for access point functionality
- ESP8266WebServer for web interface and API
- ESP8266HTTPUpdateServer for OTA updates
- DNSServer for captive portal functionality

### 11.2 Data Storage
- Use EEPROM or SPIFFS for configuration storage
- Implement a simple key-value storage system for flexibility

### 11.3 Network Scanning
- Use ESP8266WiFi library to perform network scans
- Implement a background task to periodically check for the main Stairled webserver

## 12. Testing and Validation

- Access point creation and stability tests
- Configuration interface functionality tests
- API endpoint tests
- OTA update process tests
- Integration tests with Stairled sensors and main system
- Long-term stability testing

## 13. Documentation Needs

- User manual for setup and operation
- API documentation for configuration and status endpoints
- Troubleshooting guide
- OTA update process guide for end-users