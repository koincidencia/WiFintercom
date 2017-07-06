#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "wifiwrapper.h"

static WiFiWrapper wifi;
static ESP8266WebServer httpServer(80);

#define OPEN_THE_GATE_URL "/openthegatebitch"

void OpenTheGateCallback()
{
    Serial.println(OPEN_THE_GATE_URL);
    String msg;
    msg = "Opening the gate!<br>";
    msg += "RSSI: " + wifi.GetRSSI() + "<br>";
    msg += "Uptime: " + String(millis()) + "ms<br>";
    httpServer.send(200, "text/html", msg);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("");

  // Connect to WiFi network
  wifi.ConnectToAP("A66 Guest", "Hello123");

  // Start the http server
  httpServer.on(OPEN_THE_GATE_URL, OpenTheGateCallback);
  httpServer.begin();
  Serial.println("httpServer started");

  Serial.println("Initialization done");
}

void loop()
{
  while (1) {
    httpServer.handleClient();
    delay(1);
  }
}
