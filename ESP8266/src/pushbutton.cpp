#include "pushbutton.h"

PushButton::PushButton(int pin, bool negated)
{
  this->pin = pin;
  this->negated = negated;
  pinMode(pin, OUTPUT);
}

void PushButton::Push()
{
  if (negated) {
    digitalWrite(pin, LOW);
  } else {
    digitalWrite(pin, HIGH);
  }
}

void PushButton::Release()
{
  if (negated) {
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }
}
