#ifndef  WIFIWRAPPER_H
#define WIFIWRAPPER_H

#include <ESP8266WiFi.h>

class WiFiWrapper {
private:
  static const int SSID_LEN = 32;
  static const int PASSWD_LEN  = 64;

public:
  void ConnectToAP(String ssid, String passwd);
  String GetRSSI();
  WiFiWrapper();
};

#endif /* end of include guard: WIFIWRAPPER */
