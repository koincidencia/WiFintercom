#include "gate.h"


void GateClosingCallback(Gate *gate)
{
  gate->CloseTheGate();
}

Gate::Gate()
{
  button = new PushButton(BUTTON_PIN, false);
  hook = new PushButton(HOOK_PIN, true);
  button->Release();
  hook->Push();
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
  // Get the telephone off the hook
  hook->Release();
  // Wait
  delay(HOOK_DELAY);
  // Push the door opening button
  button->Push();
  Serial.println("Gate opened");
  // Wait
  delay(BUTTON_RELEASE_TIMEOUT);
  // Release the button
  button->Release();
  // Wait
  delay(HOOK_DELAY);
  // Put the telephone on the hook again
  hook->Push();
  Serial.println("Gate closed");
}
