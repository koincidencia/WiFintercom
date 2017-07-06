# WiFintercom

WiFi enabled [Fermax LOFT telephone](http://www.fermax.com/uk/pro/products/FM-6-systems/SF-58-duox/PR-756-duox-basic-loft-telephone.html).

## Problems
- No support for WPA2 PEAP
  - Maybe the esp8266 sdk works?!
  - https://github.com/esp8266/Arduino/issues/1032
  - https://www.hallgeirholien.no/post/esp8266-eap/
  - Skip this part :D
- Hangup
  - also needed to open the gate...
  - desoltering the switch to reverse engineer
