#ifndef WIFIWRAPPER_H
#define WIFIWRAPPER_H

#include <ESP8266WiFi.h>

extern "C" {
  #include "user_interface.h"
  #include "wpa2_enterprise.h"
  #include "Arduino.h"
}

class WiFiWrapper {
private:
  static const int SSID_LEN = 32;
  static const int USRN_LEN  = 64;
  static const int PASSWD_LEN  = 64;

public:
  void ConnectToAP(String ssid, String passwd);
  void ConnectToAP(String ssid, String usrname, String passwd);
  void ScanNetworks();
  String GetRSSI();
  WiFiWrapper();
};

#endif /* end of include guard: WIFIWRAPPER */
