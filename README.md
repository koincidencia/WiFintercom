# WiFintercom

WiFi enabled [Fermax LOFT telephone](http://www.fermax.com/uk/pro/products/FM-6-systems/SF-58-duox/PR-756-duox-basic-loft-telephone.html).

## Problems
- No support for WPA2 PEAP
  - Maybe the esp8266 sdk works?!
  - https://github.com/esp8266/Arduino/issues/1032
  - https://www.hallgeirholien.no/post/esp8266-eap/
  - Skip this part :D
  - solution
  ```
    [env:esp12e]
    platform = espressif8266_stage
    board = esp12e
    framework = arduino
    board_f_flash = 80000000L
    upload_speed = 921600
    ```
- Hangup
  - also needed to open the gate...
  - desoldering the switch to reverse engineer
  - after that a small piece of solder dropped on a PLL IC
  - the IC and the PCB is finally dead :(
  - but we had another phone!
