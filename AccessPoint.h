/**
 * @file AccessPoint.h
 * @brief WiFi Access Point management for Stairled AP
 * 
 * This class is responsible for:
 * - Creating and managing the WiFi Access Point
 * - Configuring DHCP for connected clients
 * - Handling connected clients
 */

#ifndef ACCESS_POINT_H
#define ACCESS_POINT_H

#include <ESP8266WiFi.h>
#include "Config.h"

class AccessPoint {
public:
  static const IPAddress AP_IP;
  static const IPAddress AP_SUBNET;
  static const IPAddress AP_GATEWAY;
  /**
   * @brief Initialize the Access Point
   * @param config Reference to the Config instance
   * @return bool True if AP setup was successful, false otherwise
   */
  bool begin(Config& config);

  /**
   * @brief Handle Access Point operations
   */
  void handle();

  /**
   * @brief Get the number of connected clients
   * @return int Number of connected clients
   */
  int getConnectedClients() const;

  /**
   * @brief Get the IP address of the Access Point
   * @return IPAddress The IP address of the AP
   */
  IPAddress getAPIP() const;

private:


  Config* config;
  
  /**
   * @brief Set up the Access Point with configured settings
   * @return bool True if setup was successful, false otherwise
   */
  bool setupAP();

  /**
   * @brief Configure DHCP for the Access Point
   */
  void setupDHCP();
};

#endif // ACCESS_POINT_H