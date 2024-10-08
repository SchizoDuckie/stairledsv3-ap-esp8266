/**
 * @file NetworkScanner.cpp
 * @brief Implementation of network scanner for detecting main Stairled system and connected clients
 */

#include "NetworkScanner.h"

bool NetworkScanner::begin(MDNSManager& mdnsManager) {
    this->mdnsManager = &mdnsManager;
    lastScanTime = 0;
    return true;
}

void NetworkScanner::handle() {
    unsigned long currentTime = millis();
    if (currentTime - lastScanTime >= SCAN_INTERVAL) {
        scanClients();
        updateMainSystemStatus();
        lastScanTime = currentTime;
    }
}

bool NetworkScanner::isMainSystemDetected() const {
    return mdnsManager->isMainSystemDiscovered();
}

IPAddress NetworkScanner::getMainSystemIP() const {
    return mdnsManager->getMainSystemIP();
}

const std::vector<ClientInfo>& NetworkScanner::getConnectedClients() const {
    return connectedClients;
}

void NetworkScanner::scanClients() {
    connectedClients.clear();
    struct station_info *station = wifi_softap_get_station_info();
    while (station) {
        ClientInfo client;
        char macStr[18];
        sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", station->bssid[0], station->bssid[1], station->bssid[2], station->bssid[3], station->bssid[4], station->bssid[5]);
        client.macAddress = String(macStr);
        client.ipAddress = IPAddress((&station->ip)->addr);
        
        // Attempt to resolve hostname using mDNS
        String hostname = mdnsManager->resolveIP(client.ipAddress);
        client.hostname = hostname.isEmpty() ? "Unknown" : hostname;

        connectedClients.push_back(client);
        station = STAILQ_NEXT(station, next);
    }
    wifi_softap_free_station_info();
}

void NetworkScanner::updateMainSystemStatus() {
    mdnsManager->discoverMainSystem();
}