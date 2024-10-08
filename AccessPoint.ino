/**
 * @file AccessPoint.cpp
 * @brief Implementation of WiFi Access Point management for Stairled AP
 */

#include "AccessPoint.h"

const IPAddress AccessPoint::AP_IP(10, 0, 0, 1);
const IPAddress AccessPoint::AP_SUBNET(255, 255, 255, 0);
const IPAddress AccessPoint::AP_GATEWAY(10, 0, 0, 1);

bool AccessPoint::begin(Config& config) {
  this->config = &config;
  return setupAP();
}

void AccessPoint::handle() {
  // Currently no periodic tasks needed
}

int AccessPoint::getConnectedClients() const {
  return WiFi.softAPgetStationNum();
}

IPAddress AccessPoint::getAPIP() const {
  return WiFi.softAPIP();
}

bool AccessPoint::setupAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(AP_IP, AP_GATEWAY, AP_SUBNET);
  
  bool success = WiFi.softAP(config->getApName().c_str(), config->getApPassword().c_str());
  
  if (success) {
    Serial.println("Access Point started");
    Serial.print("SSID: ");
    Serial.println(config->getApName());
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
    
    // The ESP8266 automatically starts a DHCP server when in AP mode
    // We don't need to explicitly set up DHCP
  } else {
    Serial.println("Failed to start Access Point");
  }
  
  return success;
}