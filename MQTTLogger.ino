/**
 * @file MQTTLogger.cpp
 * @brief Implementation of MQTT logging functionality for Stairled AP
 */

#include "MQTTLogger.h"

bool MQTTLogger::begin(Config& configRef, NetworkScanner& networkScannerRef) {
    config = &configRef;
    networkScanner = &networkScannerRef;

    // Initialize the PubSubClient with WiFi client.
    mqttClient.setClient(wifiClient);
    
    // Try connecting to the broker during setup.
    return connectToBroker();
}

void MQTTLogger::handle() {
    // If already connected, maintain the connection.
    if (mqttClient.connected()) {
        mqttClient.loop();  // Required for the MQTT client.
    } else {
        // Retry connection every 30 seconds.
        if (millis() - lastConnectionAttempt > CONNECTION_RETRY_INTERVAL) {
            lastConnectionAttempt = millis();
            connectToBroker();  // Try to reconnect.
        }
    }
}

bool MQTTLogger::isConnected() const {
  return const_cast<PubSubClient&>(mqttClient).connected();
}

void MQTTLogger::log(const String& message) {
    // If not connected, log message to the blackhole (ignore).
    if (!isConnected()) {
        return;
    }

    // Publish the message to the MQTT topic.
    mqttClient.publish(config->getMqttTopic().c_str(), message.c_str());
}

bool MQTTLogger::connectToBroker() {
    // Fetch server IP and port from the configuration.
    IPAddress serverIP = config->getMqttServerIP();
    if (!serverIP) {
        Serial.println("MQTT Server IP not found, aborting connection attempt.");
        return false;
    }

    mqttClient.setServer(serverIP, config->getMqttPort());

    // Generate a unique client ID using random function.
    String clientId = "StairledAP-" + String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected to MQTT broker.");
        return true;
    } else {
        Serial.print("Failed to connect to MQTT broker, rc=");
        Serial.println(mqttClient.state());
        return false;
    }
}
