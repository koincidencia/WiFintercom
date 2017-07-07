#include "gate.h"

void TickerCallback(Gate *gate)
{
  switch(gate->state) {
    case 0: // Hook was off and timout occurred
      gate->ticker.detach();
      gate->state++;
      // Push the door opening button
      gate->button->Push();
      Serial.println("Gate opened");
      gate->ticker.attach(gate->BUTTON_RELEASE_TIMEOUT, TickerCallback, gate);
      break;
    case 1: // Hook was off, button was pressed and timeout occurred
      gate->ticker.detach();
      gate->state++;
      // Release the button
      gate->button->Release();
      gate->ticker.attach(gate->HOOK_DELAY, TickerCallback, gate);
      break;
    case 2: // Hook was off, button was released and timeout occurred
      gate->ticker.detach();
      gate->state = 0;
      // Put the telephone on the hook again
      gate->hook->Push();
      Serial.println("Gate closed");
      digitalWrite(LED_PIN, HIGH);
      break;
    default:
      gate->state = 0;
      break;
  }
}

Gate::Gate()
{
  button = new PushButton(BUTTON_PIN, false);
  hook = new PushButton(HOOK_PIN, true);
  button->Release();
  hook->Push();
  state = 0;

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}

Gate::~Gate()
{
  delete button;
  delete hook;
}

void Gate::OpenTheGate()
{
  digitalWrite(LED_PIN, LOW);
  // Get the telephone off the hook
  hook->Release();
  // Start the first delay
  ticker.attach(HOOK_DELAY, TickerCallback, this);
}
