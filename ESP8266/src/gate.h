#ifndef GATE_H
#define GATE_H

#include <Arduino.h>
#include <Ticker.h>
#include "pushbutton.h"

class Gate {
private:
  const int BUTTON_PIN = 14;
  const int HOOK_PIN = 12;
  const float BUTTON_RELEASE_TIMEOUT = 1000;
  const float HOOK_DELAY = 1500;
  PushButton *button;
  PushButton *hook;

public:
  Gate();
  ~Gate();
  void OpenTheGate();
};

#endif /* end of include guard: GATE_H */
