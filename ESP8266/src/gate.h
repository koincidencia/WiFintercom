#ifndef GATE_H
#define GATE_H

#include <Arduino.h>
#include <Ticker.h>

class Gate {
private:
  const int SWITCH_PIN = 14;
  const float CLOSE_TIMOUT = 1;
  Ticker gateClosingTicker;

public:
  Gate();
  void OpenTheGate();
  void CloseTheGate();
};

#endif /* end of include guard: GATE_H */
