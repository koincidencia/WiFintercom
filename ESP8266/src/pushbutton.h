#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <Arduino.h>

class PushButton {
private:
  int pin;
  bool negated;
public:
  PushButton(int pin, bool negated);
  void Push();
  void Release();
};

#endif /* end of include guard: PUSHBUTTON_H */
