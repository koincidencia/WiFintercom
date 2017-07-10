#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "wifiwrapper.h"
#include "gate.h"
#include "passwd.h"
#include "udpBroadcaster.h"
#include "wpa2_enterprise.h"

#define OPEN_THE_GATE_URL "/openthegatebitch"
#define SET_PASSWD_URL    "/setpasswd"
#define PASSWD_ID           "passwd"
#define PASSWD_ADDR         0
#define PASSWD_ACCEPT_PIN   0
#define BROADCAST_PORT      2390
#define HOSTNAME            "TOTORO"

static WiFiWrapper wifi;
static ESP8266WebServer *httpServer;
static Gate *gate;
static Passwd *passwd;
static UDPBroadcaster *broadcaster;

void OpenTheGateCallback()
{
    Serial.println(OPEN_THE_GATE_URL);
    String msg;
    // Get passwd
    String req = httpServer->arg(PASSWD_ID);
    if (req.length() <= 0) {
      Serial.println("No password entered");
      msg += "No password entered<br>";
    } else {
      if (req != passwd->GetPasswd()) {
        Serial.println("Invalid password");
        msg += "Invalid password<br>";
      } else {
        gate->OpenTheGate();
        msg = "Opening the gate!<br>";
      }
    }
    // Send HTTP response
    msg += "RSSI: " + wifi.GetRSSI() + "<br>";
    msg += "Uptime: " + String(millis()) + "ms<br>";
    httpServer->send(200, "text/html", msg);
}

void SetPasswdCallback()
{
    Serial.println(SET_PASSWD_URL);
    String msg;
    String req = httpServer->arg(PASSWD_ID);

    if (req.length() <= 0) {
      Serial.println("Invalid passwd parameter");
      msg += "Invalid passwd parameter<br>";
    } else {
      int err = passwd->SetPasswd(req);
      if (err == 0) {
        Serial.println("New password is set");
        msg += "New password is set<br>";
      } else {
        Serial.println("Password not set: " + passwd->GetError(err));
        msg += "Password not set: " + passwd->GetError(err) + "<br>";
      }
    }

    httpServer->send(200, "text/html", msg);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Startup...");

  // Creating passwd object
  passwd = new Passwd(PASSWD_ADDR);

  // Passwd reset pin init
  pinMode(PASSWD_ACCEPT_PIN, INPUT_PULLUP);

  // Creating a gate
  gate = new Gate();

  // Connect to WiFi network
  wifi.ConnectToAP("A66 Guest", "Hello123");

  // Start the http server
  httpServer = new ESP8266WebServer(80);
  httpServer->on(OPEN_THE_GATE_URL, OpenTheGateCallback);
  httpServer->on(SET_PASSWD_URL, SetPasswdCallback);
  httpServer->begin();
  Serial.println("httpServer started");

  // Initialize broadcaster
  broadcaster = new UDPBroadcaster(BROADCAST_PORT);

  // Init done
  Serial.println("Initialization done");
}

void loop()
{
  while (1) {
    httpServer->handleClient();
    broadcaster->BroadcastHandler(HOSTNAME);
    if (digitalRead(PASSWD_ACCEPT_PIN) == LOW) {
        passwd->DisableWriteProtection();
        delay(1000);
    }
    delay(1);
  }
}
