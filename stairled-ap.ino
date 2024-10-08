/**
 * @file stairled-ap.ino
 * @brief Main entry point for the Stairled Access Point firmware
 * 
 * This file initializes and manages the core components of the Stairled AP:
 * - Configuration management
 * - Access Point operations
 * - Web server for configuration interface
 * - OTA updates
 * - Network scanning for main Stairled system
 * - Captive portal functionality
 * - MQTT logging
 * - Failsafe mechanism
 * - MDNS management
 */

#include <Arduino.h>
#include "Config.h"
#include "MQTTLogger.h"
#include "NetworkScanner.h"
#include "WebServer.h"
#include "CaptivePortal.h"
#include "OTAUpdater.h"
#include "MDNSManager.h"
#include "AccessPoint.h"

// Use the existing LED_BUILTIN constant instead of a custom status LED pin.
#define RESET_BUTTON_PIN 12
#define RESET_HOLD_TIME 5000  // 5 seconds for reset.

// Global system components.
Config config;
MQTTLogger mqttLogger;
WebServer webServer;
OTAUpdater otaUpdater;
NetworkScanner networkScanner;
CaptivePortal captivePortal;
MDNSManager mdnsManager;
AccessPoint accessPoint;

unsigned long resetButtonPressTime = 0;
bool resetButtonPressed = false;

void setup() {
    Serial.begin(115200);
    Serial.println("Stairled AP starting...");

    // Configure pins.
    pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH); // LED off.

    // Initialize system components.
    if (!config.begin(mdnsManager)) {
        Serial.println("Config initialization failed.");
    }

    if (!networkScanner.begin(mdnsManager)) {
        Serial.println("Network Scanner initialization failed.");
    }

    if (!webServer.begin(config, mdnsManager, networkScanner, otaUpdater)) {
        Serial.println("Web Server initialization failed.");
    }
    
    if (!captivePortal.begin(webServer, mdnsManager)) {
        Serial.println("Captive Portal initialization failed.");
    }

    Serial.println("Initializing Access Point...");
    if (!accessPoint.begin(config)) {
      Serial.println("Access Point initialization failed.");
    } else {
      Serial.println("Access Point initialized successfully.");
    }

    if (!mqttLogger.begin(config, networkScanner)) {
        Serial.println("MQTT Logger initialization failed.");
    }

    if (!mdnsManager.begin()) {
        Serial.println("Error setting up MDNS responder!");
    }
}

void loop() {
    mdnsManager.handle();
    captivePortal.handle();
    webServer.handle();
    otaUpdater.handle();
    networkScanner.handle();
    
    mqttLogger.handle();
    handleResetButton();
}

void handleResetButton() {
    // Simple reset button logic.
    if (digitalRead(RESET_BUTTON_PIN) == LOW) {
        if (!resetButtonPressed) {
            resetButtonPressed = true;
            resetButtonPressTime = millis();
        } else if (millis() - resetButtonPressTime >= RESET_HOLD_TIME) {
            Serial.println("Resetting to default settings...");
            digitalWrite(LED_BUILTIN, LOW); // LED on.
            delay(1000);
            digitalWrite(LED_BUILTIN, HIGH); // LED off.
            ESP.restart();  // Reset the ESP device.
        }
    } else {
        resetButtonPressed = false;
    }
}
