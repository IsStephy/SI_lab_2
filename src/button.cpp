#include "Button.h"
#include <Arduino.h>

// GPIO pin number this button is connected to
int _pin;

// Constructs a Button and stores the associated GPIO pin number
Button::Button(int pin) {
    _pin = pin;
}

// Configures the pin as INPUT_PULLUP
// The internal pull-up resistor keeps the pin HIGH when idle
// Button should connect the pin to GND when pressed
void Button::Init() {
    pinMode(_pin, INPUT_PULLUP);  // Button connected to GND
}

// Reads the current state of the button
// Returns true if the button is pressed (pin reads LOW), false otherwise
bool Button::IsPressed() {
    return digitalRead(_pin) == LOW;  // Active LOW
}
