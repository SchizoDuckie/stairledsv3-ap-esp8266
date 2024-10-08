/**
 * @file MQTTLogger.h
 * @brief MQTT logging functionality for Stairled AP.
 *
 * This class is responsible for:
 * - Connecting to the MQTT broker.
 * - Sending log messages to the MQTT topic.
 * - Handling connection errors gracefully and silently if needed.
 */

#ifndef MQTT_LOGGER_H
#define MQTT_LOGGER_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "Config.h"
#include "NetworkScanner.h"

class MQTTLogger {
public:
    /**
     * @brief Initialize the MQTT logger.
     * @param config Reference to the Config instance.
     * @param networkScanner Reference to the NetworkScanner instance.
     * @return bool True if initialization was successful, false otherwise.
     */
    bool begin(Config& config, NetworkScanner& networkScanner);

    /**
     * @brief Handle MQTT connection and message publishing.
     * This function should be called frequently in the main loop.
     */
    void handle();

    /**
     * @brief Send a log message to the MQTT topic.
     * If the system is not connected, the message will be discarded (blackholed).
     * @param message The log message to send.
     */
    void log(const String& message);

    /**
     * @brief Check if MQTT is connected.
     * @return bool True if connected, false otherwise.
     */
    bool isConnected() const;

private:
    PubSubClient mqttClient;
    WiFiClient wifiClient;
    Config* config;
    NetworkScanner* networkScanner;
    unsigned long lastConnectionAttempt;
    static const unsigned long CONNECTION_RETRY_INTERVAL = 30000; // 30 seconds.

    /**
     * @brief Attempt to connect to the MQTT broker.
     * @return bool True if connection was successful, false otherwise.
     */
    bool connectToBroker();
};

#endif // MQTT_LOGGER_H
