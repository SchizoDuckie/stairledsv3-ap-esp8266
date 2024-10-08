/**
 * @file CaptivePortal.h
 * @brief Captive portal functionality for Stairled AP
 * 
 * This class is responsible for:
 * - Implementing captive portal behavior
 * - Redirecting clients to configuration interface or main system
 * - Working in conjunction with the WebServer class
 */

#ifndef CAPTIVE_PORTAL_H
#define CAPTIVE_PORTAL_H

#include "ESPAsyncDNSServer.h"
#include <ESP8266WiFi.h>
#include "WebServer.h"
#include "MDNSManager.h"


class CaptivePortal {
public:
  /**
   * @brief Initialize the captive portal
   * @param webServer Reference to the WebServer instance
   * @param mdnsManager Reference to the MDNSManager instance
   * @return bool True if initialization was successful, false otherwise
   */
  bool begin(WebServer& webServer, MDNSManager& mdnsManager);

  /**
   * @brief Handle captive portal operations
   */
  void handle();

private:
  AsyncDNSServer dnsServer;
  WebServer* webServer;
  MDNSManager* mdnsManager;
  static const byte DNS_PORT = 53;


  /**
   * @brief Set up DNS server for captive portal
   */
  void setupDNSServer();

  /**
   * @brief Log DNS request information
   * @param query The DNS query string
   */
  void logDNSRequest(const String& query) {
    Serial.print("DNS query received: ");
    Serial.println(query);
  }

  /**
   * @brief Custom DNS handler for logging DNS queries
   */
  void customDNSServerHandler(const String& domain) {
    logDNSRequest(domain);
  }
};

#endif // CAPTIVE_PORTAL_H