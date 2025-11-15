#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include <WiFi.h>
#include <WebServer.h>

TFT_eSPI tft;

// --- TOUCH (HSPI) --- not really useful
#define TOUCH_CS 33
XPT2046_Touchscreen ts(TOUCH_CS);
SPIClass touchSPI(HSPI);

// --- TOUCH CALIBRATION --- dont need that
#define TS_MINX 3844
#define TS_MAXX  305
#define TS_MINY 3880
#define TS_MAXY  250

// --- SAVED NETWORKS --- replace with your ssid/password, you dont need to fill all
const char* knownNetworks[][2] = {
  {"network1",     "Pass1"},
  {"network2",      "Pass2"},
  {"network3",      "Pass3"},
  {nullptr, nullptr}
};

// --- Web Server ---
WebServer server(80); //80 is default

// --- Auto-Scan ---
#define SCAN_INTERVAL 30000 /scan wifi every 30s if not connected to any
unsigned long lastScan = 0;
unsigned long connectTime = 0;
bool isConnected = false;

String currentIP = "No WiFi";
String currentSSID = "";

void setup() {
  Serial.begin(115200);
  Serial.println("CYD Auto WiFi + Web Server");

  // --- TOUCH ---
  touchSPI.begin(14, 12, 13, TOUCH_CS);
  ts.begin(touchSPI);
  ts.setRotation(1);

  // --- TFT ---
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(&FreeSansBold12pt7b);

  drawHomeScreen();

  lastScan = millis() - SCAN_INTERVAL + 5000;
}

void loop() {
  // --- AUTO-SCAN IF NOT CONNECTED ---
  if (!isConnected && millis() - lastScan >= SCAN_INTERVAL) {
    scanAndConnect();
    lastScan = millis();
  }

  // --- RECONNECT IF LOST ---
  if (isConnected && WiFi.status() != WL_CONNECTED) {
    isConnected = false;
    currentIP = "No WiFi";
    currentSSID = "";
    updateStatus("WiFi lost. Re scanning.");
    lastScan = millis();
  }

  server.handleClient();
  delay(10);
}

// --- SCAN & CONNECT ---
void scanAndConnect() {
  updateStatus("scanning...");

  // FIXED: DECLARE 'n' BEFORE USING IT
  int n = WiFi.scanNetworks();
  if (n == 0) {
    updateStatus(" ");
    return;
  }

  int bestRSSI = -100;
  String bestSSID = "";
  String bestPass = "";

  tft.fillRect(0, 80, 480, 200, TFT_BLACK);
  tft.setCursor(20, 100);
  tft.setTextColor(TFT_YELLOW);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.println(" ");

  for (int i = 0; i < n && i < 7; ++i) {
    String ssid = WiFi.SSID(i);
    int rssi = WiFi.RSSI(i);
    bool saved = false;
    String pass = "";

    for (int k = 0; knownNetworks[k][0]; ++k) {
      if (ssid == knownNetworks[k][0]) {
        saved = true;
        pass = knownNetworks[k][1];
        if (rssi > bestRSSI) {
          bestRSSI = rssi;
          bestSSID = ssid;
          bestPass = pass;
        }
        break;
      }
    }

    tft.setTextColor(saved ? TFT_GREEN : TFT_DARKGREY);
    tft.printf("  %s (%d dBm)%s\n", ssid.c_str(), rssi, saved ? " [SAVED]" : "");
  }

  if (bestSSID == "") {
    updateStatus("none");
    return;
  }

  updateStatus("connecting to  " + bestSSID + "...");
  WiFi.begin(bestSSID.c_str(), bestPass.c_str());

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    isConnected = true;
    currentSSID = bestSSID;
    currentIP = WiFi.localIP().toString();
    connectTime = millis();
    updateStatus("Connected! IP: " + currentIP);
    Serial.println("Connected to " + bestSSID + " - IP: " + currentIP);
    startWebServer();
    drawConnectedScreen();
  } else {
    updateStatus("fail");
  }
}

// --- WEB SERVER ---
void startWebServer() {
  server.on("/", HTTP_GET, []() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>server</title>"; //page tiltle
    html += "<style>body{font-family:Arial;background:#111;color:#0f0;text-align:center;padding:50px;}</style>";
    html += "</head><body>";
    html += "<h1>WiFi connected</h1>";
    html += "<p><strong>IP:</strong> " + currentIP + "</p>";
    html += "<p><a href='/event'>voir Info</a></p>";
    html += "<p>server on</p>";
    html += "</body></html>";
    server.send(200, "text/html", html);  /successful request
  });

  server.on("/info", HTTP_GET, []() { //server on http://ip/info
    unsigned long uptime = (millis() - connectTime) / 1000;
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>server info</title>";
    html += "<style>body{font-family:Arial;background:#222;color:#0ff;text-align:center;padding:30px;}</style>";
    html += "</head><body>";
    html += "<h1>device Info</h1>";
    html += "<p><strong>SSID:</strong> " + currentSSID + "</p>";
    html += "<p><strong>IP:</strong> " + currentIP + "</p>";
    html += "<p><strong>RSSI :</strong> " + String(WiFi.RSSI()) + " dBm</p>";
    html += "<p><strong>UP:</strong> " + String(uptime) + " sec</p>";
    html += "<p><a href='/'>return</a></p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  server.begin();
  Serial.println("Web Server: http://" + currentIP);
}

// --- UI ---
void drawHomeScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.setCursor(20, 40);
  tft.println("WiFi:");

  tft.setTextColor(TFT_CYAN);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setCursor(20, 300);
  tft.println("30s");
}

void drawConnectedScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.setCursor(20, 80);
  tft.println("WiFi");

  tft.setCursor(20, 130);
  tft.setTextColor(TFT_YELLOW);
  tft.printf("ssid: %s", currentSSID.c_str());

  tft.setCursor(20, 180);
  tft.printf("IP: %s", currentIP.c_str());

  tft.setCursor(20, 230);
  tft.printf("RSSI: %d dBm", WiFi.RSSI());

  tft.setTextColor(TFT_CYAN);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setCursor(20, 300);
  tft.println("Web: http://" + currentIP);
}

void updateStatus(String msg) {
  tft.fillRect(0, 280, 480, 40, TFT_BLACK);
  tft.setCursor(20, 285);
  tft.setTextColor(TFT_YELLOW);
  tft.setFreeFont(&FreeSansBold12pt7b);
  tft.println(msg);
}
