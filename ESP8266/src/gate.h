#ifndef GATE_H
#define GATE_H

#include <Arduino.h>
#include <Ticker.h>
#include "pushbutton.h"

#define LED_PIN             2

class Gate {
private:
  const int BUTTON_PIN = 14;
  const int HOOK_PIN = 12;

public:
  Gate();
  ~Gate();
  void OpenTheGate();

  const float BUTTON_RELEASE_TIMEOUT = 1;
  const float HOOK_DELAY = 1.5;
  PushButton *button;
  PushButton *hook;
  Ticker ticker;
  int state;
};

#endif /* end of include guard: GATE_H */
