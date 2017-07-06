#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "wifiwrapper.h"
#include "gate.h"

#define OPEN_THE_GATE_URL "/openthegatebitch"

static WiFiWrapper wifi;
static ESP8266WebServer *httpServer;
static Gate *gate;

void OpenTheGateCallback()
{
    Serial.println(OPEN_THE_GATE_URL);
    // Send HTTP response
    String msg;
    msg = "Opening the gate!<br>";
    msg += "RSSI: " + wifi.GetRSSI() + "<br>";
    msg += "Uptime: " + String(millis()) + "ms<br>";
    httpServer->send(200, "text/html", msg);
    // Open the gate
    gate->OpenTheGate();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Startup...");

  // Creating a gate
  gate = new Gate();

  // Connect to WiFi network
  wifi.ConnectToAP("A66 Guest", "Hello123");

  // Start the http server
  httpServer = new ESP8266WebServer(80);
  httpServer->on(OPEN_THE_GATE_URL, OpenTheGateCallback);
  httpServer->begin();
  Serial.println("httpServer started");

  // Init done
  Serial.println("Initialization done");
}

void loop()
{
  while (1) {
    httpServer->handleClient();
    delay(1);
  }
}
