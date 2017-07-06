#include "gate.h"


void GateClosingCallback(Gate *gate)
{
  gate->CloseTheGate();
}

Gate::Gate()
{
  pinMode(SWITCH_PIN, OUTPUT);
  CloseTheGate();
}

void Gate::CloseTheGate()
{
  digitalWrite(SWITCH_PIN, LOW);
  gateClosingTicker.detach();
  Serial.println("Gate closed");
}

void Gate::OpenTheGate()
{
  digitalWrite(SWITCH_PIN, HIGH);
  gateClosingTicker.attach(CLOSE_TIMOUT, GateClosingCallback, this);
  Serial.println("Gate opened");
}
