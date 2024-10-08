/**
 * @file NetworkScanner.h
 * @brief Network scanner for detecting main Stairled system and connected clients
 * 
 * This class is responsible for:
 * - Periodically scanning for the main Stairled webserver
 * - Maintaining a list of connected clients
 * - Providing network status information
 */

#ifndef NETWORK_SCANNER_H
#define NETWORK_SCANNER_H

#include <ESP8266WiFi.h>
#include <vector>
#include "MDNSManager.h"

struct ClientInfo {
    String macAddress;
    IPAddress ipAddress;
    String hostname;
};

class NetworkScanner {
public:
    /**
     * @brief Initialize the network scanner
     * @param mdnsManager Reference to the MDNSManager instance
     * @return bool True if initialization was successful, false otherwise
     */
    bool begin(MDNSManager& mdnsManager);

    /**
     * @brief Handle network scanning operations
     */
    void handle();

    /**
     * @brief Check if main Stairled system is detected
     * @return bool True if main system is detected, false otherwise
     */
    bool isMainSystemDetected() const;

    /**
     * @brief Get the IP address of the main Stairled system
     * @return IPAddress The IP address of the main system, or INADDR_NONE if not detected
     */
    IPAddress getMainSystemIP() const;

    /**
     * @brief Get the list of connected clients
     * @return const std::vector<ClientInfo>& Reference to the list of connected clients
     */
    const std::vector<ClientInfo>& getConnectedClients() const;

private:
    MDNSManager* mdnsManager;
    std::vector<ClientInfo> connectedClients;
    unsigned long lastScanTime;
    static const unsigned long SCAN_INTERVAL = 30000; // 30 seconds

    /**
     * @brief Scan for connected clients
     */
    void scanClients();

    /**
     * @brief Update the main system status
     */
    void updateMainSystemStatus();
};

#endif // NETWORK_SCANNER_H