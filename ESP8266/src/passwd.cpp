#include "passwd.h"

Passwd::Passwd(int addr)
{
  this->addr = addr;

  EEPROM.begin(EEPROM_MEMORY_SIZE);
  char tmp[EEPROM_MEMORY_SIZE + 1];
  for (int i = 0; i < EEPROM_MEMORY_SIZE; i++) {
    tmp[i] = EEPROM.read(addr + i);
  }
  passwd = String(tmp);
  EEPROM.end();

  writeProtection = true;
  Serial.println("Password is loaded from EEPROM");
  Serial.println(passwd);
}

String Passwd::GetPasswd()
{
  return passwd;
}

void Passwd::DisableWriteProtection()
{
  writeProtection = false;
  Serial.println("Password write protection disabled");
}

int Passwd::SetPasswd(String passwd)
{
  if (writeProtection) {
    Serial.println("Password is write protected");
    return -1;
  }

  if (passwd.length() > EEPROM_MEMORY_SIZE) {
    Serial.println("Password is too long");
    return -2;
  }

  char tmp[EEPROM_MEMORY_SIZE + 1];
  passwd.toCharArray(tmp, EEPROM_MEMORY_SIZE);

  EEPROM.begin(EEPROM_MEMORY_SIZE);
  int i;
  for (i = 0; i <= passwd.length(); i++) {
    EEPROM.write(addr + i, passwd[i]);
  }
  EEPROM.write(addr + i, '\0');
  EEPROM.commit();
  EEPROM.end();
  writeProtection = true;
  this->passwd = passwd;

  Serial.println("Password is saved to EEPROM");
  return 0;
}

String Passwd::GetError(int err)
{
  switch(err)
  {
    case -1:
      return "Password is write protected";
      break;
    case -2:
      return "Password was too long";
      break;
    default:
      return "No error";
      break;
  }
}
