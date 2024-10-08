/**
 * @file MDNSManager.cpp
 * @brief Implementation of MDNS management for Stairled AP
 */

#include "MDNSManager.h"

const char* MDNSManager::MAIN_SYSTEM_HOSTNAME = "stairled-server";

bool MDNSManager::begin() {
  if (!MDNS.begin(currentHostname.c_str())) {
    Serial.println("Error setting up MDNS responder!");
    return false;
  }
  
  MDNS.addService("http", "tcp", 80);
  mainSystemDiscovered = false;
  mainSystemIP = INADDR_NONE;
  return true;
}

void MDNSManager::handle() {
  MDNS.update();
  discoverMainSystem();
}

bool MDNSManager::setHostname(const String& hostname) {
  if (hostname == currentHostname) {
    return true;  // Hostname is already set to this value
  }

  currentHostname = hostname;
  
  // If MDNS is already running, we need to stop it and restart with the new hostname
  MDNS.end();
  if (!MDNS.begin(currentHostname.c_str())) {
    Serial.println("Error setting new hostname for MDNS responder!");
    return false;
  }

  MDNS.addService("http", "tcp", 80);
  return true;
}

bool MDNSManager::isMainSystemDiscovered() const {
  return mainSystemDiscovered;
}

IPAddress MDNSManager::getMainSystemIP() const {
  return mainSystemIP;
}

IPAddress MDNSManager::resolveHostname(const String& hostname) {
  IPAddress ip = INADDR_NONE;
  int n = MDNS.queryService("http", "tcp");
  for (int i = 0; i < n; i++) {
    if (MDNS.hostname(i) == hostname) {
      ip = MDNS.IP(i);
      break;
    }
  }
  return ip;
}

String MDNSManager::resolveIP(const IPAddress& ip) {
  String hostname;
  if (MDNS.begin("esp8266")) {
    hostname = MDNS.hostname(ip);
  }
  return hostname;
}

void MDNSManager::discoverMainSystem() {
  if (!mainSystemDiscovered) {
    IPAddress ip = resolveHostname(MAIN_SYSTEM_HOSTNAME);
    if (ip != INADDR_NONE) {
      mainSystemDiscovered = true;
      mainSystemIP = ip;
      Serial.print("Main Stairled system discovered at: ");
      Serial.println(mainSystemIP);
    }
  }
}

