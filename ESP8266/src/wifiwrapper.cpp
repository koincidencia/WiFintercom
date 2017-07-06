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

String WiFiWrapper::GetRSSI()
{
  String rssi(WiFi.RSSI());
  return rssi;
}
