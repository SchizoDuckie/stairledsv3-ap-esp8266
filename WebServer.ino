#include "WebServer.h"


const char PROGMEM html_header[] =
  "<html lang='en'>"
  "<head>"
  "<meta charset='UTF-8'>"
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "<title>Stairled Configuration</title>"
  "<style>"
  "body { font-family: Arial, sans-serif; background-color: #f4f4f4; color: #333; margin: 0; padding: 0; }"
  "h1 { text-align: center; margin-top: 20px; color: #4CAF50; }"
  "form { background-color: white; padding: 20px; margin: 0 auto; width: 80%; max-width: 600px; border-radius: 8px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); }"
  "input[type='text'], input[type='password'], input[type='number'], input[type='file'] {"
  "  width: 100%; padding: 12px; margin: 10px 0; border: 1px solid #ddd; border-radius: 4px; }"
  "input[type='submit'] { background-color: #4CAF50; color: white; border: none; padding: 12px 20px; cursor: pointer; border-radius: 4px; font-size: 16px; }"
  "input[type='submit']:hover { background-color: #45a049; }"
  "label { font-weight: bold; margin-bottom: 8px; display: block; }"
  ".container { max-width: 800px; margin: 0 auto; padding: 20px; }"
  ".footer { text-align: center; margin-top: 20px; font-size: 14px; color: #777; }"
  "</style>"
  "</head><body>";

const char PROGMEM html_footer[] =
  "<div class='footer'>"
  "<p>Stairled Configuration &copy; 2024. All Rights Reserved.</p>"
  "</div></body></html>";

const char PROGMEM html_form[] =
  "<div class='container'>"
  "<h1>Stairled AP Configuration</h1>"
  "<form action='/api/config' method='post'>"
  "<label for='ap_name'>AP Name:</label><input type='text' name='ap_name' value='%s'>"
  "<label for='ap_pass'>AP Password:</label><input type='password' name='ap_pass' value='%s'>"
  "<label for='mqtt_server'>MQTT Server:</label><input type='text' name='mqtt_server' value='%s'>"
  "<label for='mqtt_port'>MQTT Port:</label><input type='number' name='mqtt_port' value='%d'>"
  "<label for='mqtt_topic'>MQTT Topic:</label><input type='text' name='mqtt_topic' value='%s'>"
  "<input type='submit' value='Save Configuration'>"
  "</form>";

const char PROGMEM html_ota_form[] =
  "<h1>Upload Firmware</h1>"
  "<form action='/api/ota' method='post' enctype='multipart/form-data'>"
  "<label for='firmware'>Firmware File:</label><input type='file' name='firmware'>"
  "<input type='submit' value='Upload Firmware'>"
  "</form>";


bool WebServer::begin(Config& config, MDNSManager& mdnsManager, NetworkScanner& networkScanner, OTAUpdater& otaUpdater) {
  this->config = &config;
  this->mdnsManager = &mdnsManager;
  this->networkScanner = &networkScanner;
  this->otaUpdater = &otaUpdater;

  server.on("/", std::bind(&WebServer::handleRoot, this));
  server.on("/api/config", HTTP_POST, std::bind(&WebServer::handleConfigApi, this));
  server.on("/api/clients", HTTP_GET, std::bind(&WebServer::handleClientsApi, this));
  server.on("/api/ota", HTTP_POST, [this]() {
    server.send(200, "text/plain", "OTA update finished");
  }, std::bind(&WebServer::handleOTAUpload, this));
  server.onNotFound(std::bind(&WebServer::handleCaptivePortal, this));

  server.on("/generate_204", std::bind(&WebServer::handleCaptivePortal, this));  // Android captive portal detection
  server.on("/fwlink", std::bind(&WebServer::handleCaptivePortal, this));       // Microsoft captive portal detection


  server.begin();
  Serial.println("WebServer started");
  return true;
}

void WebServer::handle() {
  server.handleClient();
}

IPAddress WebServer::getIP() const {
  return WiFi.softAPIP();
}

void WebServer::handleRoot() {
  Serial.println("Handling root request");
  if (redirectToMainSystem()) {
    Serial.println("Redirected to main system");
    return;
  }

  // Construct HTML page using snprintf_P for efficient memory usage
  char page[1024];
  snprintf_P(page, sizeof(page), html_form,
             config->getApName().c_str(),
             config->getApPassword().c_str(),
             config->getMqttServerName().c_str(),
             config->getMqttPort(),
             config->getMqttTopic().c_str());

  String fullPage = String(FPSTR(html_header)) + String(page) + String(FPSTR(html_ota_form)) + String(FPSTR(html_footer));
  server.send(200, "text/html", fullPage);
  Serial.println("Sent configuration page");
}

void WebServer::handleConfigApi() {
  Serial.println("Handling config API request");
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
    Serial.println("Method not allowed for config API");
    return;
  }

  if (server.hasArg("ap_name")) config->setApName(server.arg("ap_name"));
  if (server.hasArg("ap_pass")) config->setApPassword(server.arg("ap_pass"));
  if (server.hasArg("mqtt_server")) config->setMqttServerName(server.arg("mqtt_server"));
  if (server.hasArg("mqtt_port")) config->setMqttPort(server.arg("mqtt_port").toInt());
  if (server.hasArg("mqtt_topic")) config->setMqttTopic(server.arg("mqtt_topic"));

  config->save();

  server.send(200, "text/plain", "Configuration updated successfully");
  Serial.println("Configuration updated");
}

void WebServer::handleOTAUpload() {
  HTTPUpload& upload = server.upload();
  
  if (upload.status == UPLOAD_FILE_START) {
    Serial.println("OTA upload started");
    if (!upload.filename.endsWith(".bin")) {
      server.send(400, "text/plain", "Invalid firmware file");
      Serial.println("Invalid firmware file");
      return;
    }
    otaUpdater->beginOTA(upload.filename);
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    otaUpdater->processOTA(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    otaUpdater->endOTA();
    Serial.println("OTA upload completed");
  } else if (upload.status == UPLOAD_FILE_ABORTED) {
    otaUpdater->abortOTA();
    server.send(500, "text/plain", "Firmware upload aborted");
    Serial.println("OTA upload aborted");
  }
}

void WebServer::handleClientsApi() {
    Serial.println("Handling clients API request");
    const auto& clients = networkScanner->getConnectedClients();
    String json = "[";
    for (const auto& client : clients) {
        if (json.length() > 1) json += ",";
        json += "{\"mac\":\"" + client.macAddress + "\",";
        json += "\"ip\":\"" + client.ipAddress.toString() + "\",";
        json += "\"hostname\":\"" + client.hostname + "\"}";
    }
    json += "]";
    server.send(200, "application/json", json);
    Serial.println("Sent clients list");
}

void WebServer::handleCaptivePortal() {
    Serial.println("Handling potential captive portal request");
    Serial.print("Request URI: ");
    Serial.println(server.uri());
    Serial.print("Host header: ");
    Serial.println(server.hostHeader());
    
    if (isCaptivePortalRequest()) {
        Serial.println("Captive portal request detected");
        if (redirectToMainSystem()) {
            Serial.println("Redirected to main system");
            return;
        }
        Serial.println("Handled as main configuration page");
        handleRoot();
        // Redirect to the configuration page
        //server.sendHeader("Location", String("http://") + toStringIP(server.client().localIP()), true);
        //server.send(302, "text/plain", "");  
        
    } else {
        Serial.println("Not a captive portal request, sending 404");
        server.send(404, "text/plain", "Not Found");
    }
}

bool WebServer::isCaptivePortalRequest() {
      String host = server.hostHeader();
  IPAddress serverIP = getIP();
  
  Serial.print("Host header: ");
  Serial.println(host);
  Serial.print("Server IP: ");
  Serial.println(serverIP.toString());

  if (host == serverIP.toString() || 
      host == "captive.apple.com" ||
      host == "connectivitycheck.gstatic.com" ||
      host == "connectivitycheck.android.com" ||
      host == "clients3.google.com") {
    return false;  // These are legitimate requests to our server
  }
  
  return true;  // All other requests are likely captive portal checks
}

bool WebServer::redirectToMainSystem() {
  if (mdnsManager->isMainSystemDiscovered()) {
    IPAddress mainSystemIP = mdnsManager->getMainSystemIP();
    Serial.print("Main system discovered. Redirecting to: ");
    Serial.println(toStringIP(mainSystemIP));
    server.sendHeader("Location", String("http://") + toStringIP(mainSystemIP), true);
    server.send(302, "text/plain", "");
    return true;
  }
  Serial.println("Main system not discovered, not redirecting");
  return false;
}

String WebServer::toStringIP(IPAddress ip) {
  return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}