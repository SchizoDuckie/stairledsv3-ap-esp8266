/**
 * @file OTAUpdater.h
 * @brief OTA update functionality for Stairled AP
 * 
 * This class is responsible for:
 * - Handling firmware updates via web interface
 * - Verifying and applying updates
 */

#ifndef OTA_UPDATER_H
#define OTA_UPDATER_H

#include <Arduino.h>
#include <ESP8266HTTPUpdateServer.h>

class OTAUpdater {
public:
  /**
   * @brief Initialize the OTA updater
   * @param server Reference to the ESP8266WebServer instance
   * @return bool True if initialization was successful, false otherwise
   */
  bool begin(ESP8266WebServer& server);

  /**
   * @brief Handle OTA update requests
   */
  void handle();

  /**
   * @brief Start the OTA update process
   * @param filename The name of the firmware file
   */
  void beginOTA(const String& filename);

  /**
   * @brief Process a chunk of the OTA update
   * @param data Pointer to the data buffer
   * @param len Length of the data buffer
   */
  void processOTA(uint8_t* data, size_t len);

  /**
   * @brief Finish the OTA update process
   */
  void endOTA();

  /**
   * @brief Abort the OTA update process
   */
  void abortOTA();

private:
  ESP8266HTTPUpdateServer httpUpdater;
  bool updateInProgress;
  String updateFilename;
};

#endif // OTA_UPDATER_H