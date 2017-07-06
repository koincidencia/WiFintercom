#include "gate.h"


void GateClosingCallback(Gate *gate)
{
  gate->CloseTheGate();
}

Gate::Gate()
{
  button = new PushButton(BUTTON_PIN, false);
  hook = new PushButton(HOOK_PIN, false);
}

Gate::~Gate()
{
  delete button;
  delete hook;
}

void Gate::CloseTheGate()
{
  Serial.println("Gate closed");
}

void Gate::OpenTheGate()
{
  Serial.println("Gate opened");
}
