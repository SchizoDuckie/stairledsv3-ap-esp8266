/**
 * @file Config.h
 * @brief Configuration management for Stairled Access Point
 * 
 * This class is responsible for:
 * - Loading and saving configuration data using the Storage class
 * - Providing access to configuration parameters
 * - Managing default configuration values
 * - Ensuring unique AP names for multiple systems on the network
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "Storage.h"
#include "MDNSManager.h"

class Config {
public:
  /**
   * @brief Initialize the configuration system
   * @param mdnsManager Reference to the MDNSManager instance
   */
  bool begin(MDNSManager& mdnsManager);

  /**
   * @brief Load configuration from storage
   */
  bool load();

  /**
   * @brief Save configuration to storage
   */
  bool save();

  // Getter and setter methods for configuration parameters
  String getApName() const;
  void setApName(const String& name);

  String getApPassword() const;
  void setApPassword(const String& password);

  String getMqttServerName() const;
  void setMqttServerName(const String& serverName);

  IPAddress getMqttServerIP();

  int getMqttPort() const;
  void setMqttPort(int port);

  String getMqttTopic() const;
  void setMqttTopic(const String& topic);

  /**
   * @brief Reset all configuration to default values
   */
  void resetToDefaults();

private:
  String apName;
  String apPassword;
  String mqttServerName;
  int mqttPort;
  String mqttTopic;

  MDNSManager* mdnsManager;

  // Default values
  static const char* DEFAULT_AP_NAME_PREFIX;
  static const char* DEFAULT_AP_PASSWORD;
  static const char* DEFAULT_MQTT_SERVER_NAME;
  static const int DEFAULT_MQTT_PORT = 1883;
  static const char* DEFAULT_MQTT_TOPIC;

  /**
   * @brief Generate a unique AP name
   * @return String A unique AP name
   */
  String generateUniqueApName();
};

extern Config config;

#endif // CONFIG_H