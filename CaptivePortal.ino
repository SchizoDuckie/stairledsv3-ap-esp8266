#include "CaptivePortal.h"
#include "AccessPoint.h"

bool CaptivePortal::begin(WebServer& webServer, MDNSManager& mdnsManager) {
  this->webServer = &webServer;
  this->mdnsManager = &mdnsManager;
  
  setupDNSServer();
  return true;
}

void CaptivePortal::handle() {
  
}

void CaptivePortal::setupDNSServer() {
  // Set up DNS server with the ESP IP address to handle all DNS requests
  dnsServer.setErrorReplyCode(AsyncDNSReplyCode::NoError);  
  dnsServer.start(DNS_PORT, "*", AccessPoint::AP_IP);

 // Intercept DNS queries and log them
  dnsServer.setTTL(60); // Set the TTL (time to live) for DNS responses (optional)

  Serial.print ("DNS Server started. Uses the following ip: for all responses:");
  Serial.println(WiFi.softAPIP());
  
}
