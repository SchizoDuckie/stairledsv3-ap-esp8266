#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESP8266WebServer.h>
#include "Config.h"
#include "MDNSManager.h"
#include "NetworkScanner.h"
#include "OTAUpdater.h"  // Add OTA support

class WebServer {
public:
  /**
   * @brief Initialize the web server
   * @param config Reference to the Config instance
   * @param mdnsManager Reference to the MDNSManager instance
   * @param networkScanner Reference to the NetworkScanner instance
   * @param otaUpdater Reference to the OTAUpdater instance
   * @return bool True if initialization was successful, false otherwise
   */
  bool begin(Config& config, MDNSManager& mdnsManager, NetworkScanner& networkScanner, OTAUpdater& otaUpdater);

  /**
   * @brief Handle incoming web requests
   */
  void handle();

  /**
   * @brief Get the IP address of the web server
   * @return IPAddress The IP address of the web server
   */
  IPAddress getIP() const;

private:
  ESP8266WebServer server;
  Config* config;
  MDNSManager* mdnsManager;
  NetworkScanner* networkScanner;
  OTAUpdater* otaUpdater;

  /**
   * @brief Handle root page request
   */
  void handleRoot();

  /**
   * @brief Handle configuration API requests
   */
  void handleConfigApi();

  /**
   * @brief Handle connected clients list API requests
   */
  void handleClientsApi();

  /**
   * @brief Handle OTA firmware upload requests
   */
  void handleOTAUpload();  // Add OTA endpoint handler

  /**
   * @brief Handle captive portal requests
   */
  void handleCaptivePortal();

  /**
   * @brief Check if the request is for a captive portal
   * @return bool True if the request is for a captive portal, false otherwise
   */
  bool isCaptivePortalRequest();

  /**
   * @brief Redirect to the main Stairled system if discovered
   * @return bool True if redirected, false if main system not discovered
   */
  bool redirectToMainSystem();

  /**
   * @brief Convert IPAddress to String
   * @param ip IPAddress to convert
   * @return String IP address in string format
   */
  String toStringIP(IPAddress ip);
};

#endif // WEB_SERVER_H