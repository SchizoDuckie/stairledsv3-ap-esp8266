/**
 * @file MDNSManager.h
 * @brief MDNS management for Stairled AP
 * 
 * This class is responsible for:
 * - Setting up and managing mDNS functionality
 * - Advertising the Stairled AP service
 * - Discovering the main Stairled system by hostname
 * - Resolving hostnames to IP addresses
 */

#ifndef MDNS_MANAGER_H
#define MDNS_MANAGER_H

#include <ESP8266mDNS.h>
#include <IPAddress.h>

class MDNSManager {
public:
  /**
   * @brief Initialize the MDNS manager
   * @return bool True if MDNS responder started successfully, false otherwise
   */
  bool begin();

  /**
   * @brief Handle MDNS operations
   */
  void handle();

  /**
   * @brief Set the hostname for this device
   * @param hostname The hostname to set
   * @return bool True if the hostname was set successfully, false otherwise
   */
  bool setHostname(const String& hostname);

  /**
   * @brief Resolve a hostname to an IP address
   * @param hostname The hostname to resolve
   * @return IPAddress The resolved IP address, or INADDR_NONE if resolution failed
   */
  IPAddress resolveHostname(const String& hostname);

  /**
   * @brief Check if the main Stairled system is discovered
   * @return bool True if main system is discovered, false otherwise
   */
  bool isMainSystemDiscovered() const;

  /**
   * @brief Get the IP address of the main Stairled system
   * @return IPAddress The IP address of the main system, or INADDR_NONE if not discovered
   */
  IPAddress getMainSystemIP() const;

  /**
   * @brief Discover the main Stairled system using mDNS
   */
  void discoverMainSystem();
  /**
    * @brief Resolve an IP address to a hostname
    * @param ip The IP address to resolve
    * @return String The resolved hostname, or empty string if not found
    */
  String resolveIP(const IPAddress& ip);

private:
  static const char* MAIN_SYSTEM_HOSTNAME;
  bool mainSystemDiscovered;
  IPAddress mainSystemIP;
  String currentHostname;


};

extern MDNSManager mdnsManager;

#endif // MDNS_MANAGER_H