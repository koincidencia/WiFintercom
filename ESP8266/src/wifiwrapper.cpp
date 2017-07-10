#include "WiFiWrapper.h"

WiFiWrapper::WiFiWrapper()
{

}

void WiFiWrapper::ConnectToAP(String ssid, String passwd)
{
  Serial.println("Connecting to " + ssid + "...");
	WiFi.mode(WIFI_STA);

	char ssid_ch[this->SSID_LEN];
	char password_ch[this->PASSWD_LEN];
	ssid.toCharArray(ssid_ch, this->SSID_LEN);
	passwd.toCharArray(password_ch, this->PASSWD_LEN);
	WiFi.begin(ssid_ch, password_ch);

	if(WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.println("WiFi Connect Failed! Rebooting...");
		delay(1000);
		ESP.restart();
	}

	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
}

void WiFiWrapper::ConnectToAP(String ssid, String usrname, String passwd)
{
  WiFi.disconnect();
  int n = WiFi.scanNetworks();
  char ssid_ch[this->SSID_LEN];
	char usrname_ch[this->USRN_LEN];
	char password_ch[this->PASSWD_LEN];
	ssid.toCharArray(ssid_ch, this->SSID_LEN);
	usrname.toCharArray(usrname_ch, this->USRN_LEN);
	passwd.toCharArray(password_ch, this->PASSWD_LEN);

  Serial.println("Connecting to " + ssid + "...");
	wifi_set_opmode(STATION_MODE);

  struct station_config wifi_config;
  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char*)wifi_config.ssid, ssid_ch);

  wifi_station_disconnect();
  int ok = wifi_station_set_config(&wifi_config);

  if (!ok) {
    Serial.println("wifi_station_set_config ERR");
    delay(2000);
  }

  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();

  if (wifi_station_set_wpa2_enterprise_auth(1)) {
    Serial.println("wifi_station_set_wpa2_enterprise_auth ERR");
    delay(2000);
  }
  if (wifi_station_set_enterprise_username((uint8*)usrname_ch, strlen(usrname_ch))) {
    Serial.println("wifi_station_set_enterprise_username ERR");
    delay(2000);
  }
  if (wifi_station_set_enterprise_password((uint8*)password_ch, strlen(password_ch))) {
    Serial.println("wifi_station_set_enterprise_password ERR");
    delay(2000);
  }
  if (!wifi_station_connect()) {
    Serial.println("wifi_station_connect ERR");
    delay(2000);
  }

    // Wait for connection AND IP address from DHCP
  Serial.println();
  Serial.println("Waiting for connection and IP Address from DHCP");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

String WiFiWrapper::GetRSSI()
{
  String rssi(WiFi.RSSI());
  return rssi;
}

void WiFiWrapper::ScanNetworks()
{
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      switch (WiFi.encryptionType(i)) {
          case ENC_TYPE_WEP:
              Serial.println("WEP");
              break;
          case ENC_TYPE_TKIP:
              Serial.println("WPA");
              break;
          case ENC_TYPE_CCMP:
              Serial.println("WPA2");
              break;
          case ENC_TYPE_NONE:
              Serial.println("None");
              break;
          case ENC_TYPE_AUTO:
              Serial.println("Auto");
              break;
          default:
              Serial.print("Other:");
              Serial.println(WiFi.encryptionType(i));
              break;
      }
      delay(10);
    }
  }
  Serial.println("");
}
