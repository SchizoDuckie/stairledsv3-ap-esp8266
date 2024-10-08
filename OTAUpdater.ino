/**
 * @file OTAUpdater.cpp
 * @brief Implementation of OTA update functionality for Stairled AP
 */

#include "OTAUpdater.h"
#include <ESP8266WebServer.h>

bool OTAUpdater::begin(ESP8266WebServer& server) {
  httpUpdater.setup(&server);
  updateInProgress = false;
  return true;
}

void OTAUpdater::handle() {
  // The ESP8266HTTPUpdateServer handles most of the work
  // We don't need to do anything here in the regular loop
}

void OTAUpdater::beginOTA(const String& filename) {
  if (!updateInProgress) {
    updateInProgress = true;
    updateFilename = filename;
    Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000);
    Serial.println("OTA update started: " + filename);
  }
}

void OTAUpdater::processOTA(uint8_t* data, size_t len) {
  if (updateInProgress) {
    Update.write(data, len);
  }
}

void OTAUpdater::endOTA() {
  if (updateInProgress) {
    if (Update.end(true)) {
      Serial.println("OTA update successful: " + updateFilename);
    } else {
      Serial.println("OTA update failed: " + String(Update.getError()));
    }
    updateInProgress = false;
    updateFilename = "";
  }
}

void OTAUpdater::abortOTA() {
  if (updateInProgress) {
    Update.end();
    updateInProgress = false;
    updateFilename = "";
    Serial.println("OTA update aborted");
  }
}