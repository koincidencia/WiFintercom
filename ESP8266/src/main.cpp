/*#include <Arduino.h>
#include "wifiwrapper.h"

WiFiWrapper wifi;

void setup()
{
  Serial.begin(115200);
  Serial.println("");

  wifi.ConnectToAP("A66 Office", "");

  Serial.println("Initialization done");
}

void loop()
{

}
*/

/**
 *  Brief:
 *    This program connects to a remote WiFi Access Point with
 * WPA2 Enterprise security
 */
/*******************************************************************/
#include <ESP8266WiFi.h>
/*******************************************************************/
extern "C" {
  #include "user_interface.h"
  #include "wpa2_enterprise.h"
}
/*******************************************************************/
#define SERIAL_BAUD_RATE      57600
#define STARTUP_DELAY_MS      1000
/*******************************************************************/
// SSID to connect to
static const char* ssid = "MY_SSID";
// Username for authentification
static const char* username = "user1";
// Password for authentification
static const char* password = "secret";
/*******************************************************************/
void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  delay(STARTUP_DELAY_MS);

  // Setting ESP into STATION mode only (no AP mode or dual mode)
  wifi_set_opmode(STATION_MODE);

  struct station_config wifi_config;

  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char*)wifi_config.ssid, ssid);

  wifi_station_set_config(&wifi_config);

  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();

  wifi_station_set_wpa2_enterprise_auth(1);
  wifi_station_set_enterprise_username((uint8*)username, strlen(username));
  wifi_station_set_enterprise_password((uint8*)password, strlen(password));

  wifi_station_connect();

  // Wait for connection AND IP address from DHCP
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Now we are connected
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
/*******************************************************************/
void loop() {
  delay(5000);
}
/*******************************************************************/
