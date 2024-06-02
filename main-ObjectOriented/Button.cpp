#include "Button.h"
#include <Arduino.h>

ButtonModule::ButtonModule() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button pin as input with internal pull-up resistor
}

bool ButtonModule::isPressed() {
  return digitalRead(BUTTON_PIN) == LOW;
}
