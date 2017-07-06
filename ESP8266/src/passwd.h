#ifndef PASSWD_H
#define PASSWD_H

#include <Arduino.h>
#include <EEPROM.h>

class Passwd {
private:
  const int EEPROM_MEMORY_SIZE = 64;
  String passwd;
  int addr;
  bool writeProtection;

public:
  Passwd(int addr);
  String GetPasswd();
  void DisableWriteProtection();
  int SetPasswd(String passwd);
  String GetError(int err);
};



#endif /* end of include guard: PASSWD_H */
