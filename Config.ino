/**
 * @file Config.cpp
 * @brief Implementation of configuration management for Stairled Access Point
 */

#include "Config.h"
#include <ESP8266WiFi.h>

// Define default values
const char* Config::DEFAULT_AP_NAME_PREFIX = "stairled-ap-";
const char* Config::DEFAULT_AP_PASSWORD = "stairledpassword";
const char* Config::DEFAULT_MQTT_SERVER_NAME = "stairled-server.local";
const char* Config::DEFAULT_MQTT_TOPIC = "stairled/ap/log";

bool Config::begin(MDNSManager& mdnsManager) {
  this->mdnsManager = &mdnsManager;
  load();
  if (apName.isEmpty()) {
    apName = generateUniqueApName();
    if (!save()) {
      Serial.println("Failed to save configuration");
      return false;
    }
  }
  return true;
}

bool Config::load() {
  apName = storage.readString("ap_name", "");
  apPassword = storage.readString("ap_pass", DEFAULT_AP_PASSWORD);
  mqttServerName = storage.readString("mqtt_server", DEFAULT_MQTT_SERVER_NAME);
  mqttPort = storage.readString("mqtt_port", String(DEFAULT_MQTT_PORT)).toInt();
  mqttTopic = storage.readString("mqtt_topic", DEFAULT_MQTT_TOPIC);
  return true;
}

bool Config::save() {
  storage.writeString("ap_name", apName);
  storage.writeString("ap_pass", apPassword);
  storage.writeString("mqtt_server", mqttServerName);
  storage.writeString("mqtt_port", String(mqttPort));
  storage.writeString("mqtt_topic", mqttTopic);
  storage.commit();
  return true;
}

String Config::getApName() const {
  return apName;
}

void Config::setApName(const String& name) {
  apName = name;
  mdnsManager->setHostname(apName);
}

String Config::getApPassword() const {
  return apPassword;
}

void Config::setApPassword(const String& password) {
  apPassword = password;
}

String Config::getMqttServerName() const {
  return mqttServerName;
}

void Config::setMqttServerName(const String& serverName) {
  mqttServerName = serverName;
}

IPAddress Config::getMqttServerIP() {
  return mdnsManager->resolveHostname(mqttServerName);
}

int Config::getMqttPort() const {
  return mqttPort;
}

void Config::setMqttPort(int port) {
  mqttPort = port;
}

String Config::getMqttTopic() const {
  return mqttTopic;
}

void Config::setMqttTopic(const String& topic) {
  mqttTopic = topic;
}

void Config::resetToDefaults() {
  apName = generateUniqueApName();
  apPassword = DEFAULT_AP_PASSWORD;
  mqttServerName = DEFAULT_MQTT_SERVER_NAME;
  mqttPort = DEFAULT_MQTT_PORT;
  mqttTopic = DEFAULT_MQTT_TOPIC;
  save();
  mdnsManager->setHostname(apName);
}

String Config::generateUniqueApName() {
  String macAddress = WiFi.macAddress();
  macAddress.replace(":", "");
  return String(DEFAULT_AP_NAME_PREFIX) + macAddress.substring(6);
}