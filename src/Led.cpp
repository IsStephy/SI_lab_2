#include "Led.h"
#include <Arduino.h>
// Constructor: saves the pin number for the LED
Led::Led(int pin) {
    _pin = pin;
}
// Initializes the LED pin
// Sets the pin as OUTPUT and turns the LED OFF by default
void Led::Init() {
    pinMode(_pin, OUTPUT);
}
// Turns the LED ON
void Led::On() {
    digitalWrite(_pin, HIGH);
}
// Turns the LED OFF
void Led::Off() {
    digitalWrite(_pin, LOW);
}

