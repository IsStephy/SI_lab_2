#ifndef BUTTON_H
#define BUTTON_H

// Represents a physical push button connected to a GPIO pin
class Button {
public:
    // Constructs a Button object for the specified GPIO pin
    Button(int pin);

    // Initializes the button pin as an input
    // Should be called once during setup before reading button state
    void Init();

    // Checks whether the button is currently pressed
    // Returns true if pressed, false otherwise
    bool IsPressed();

private:
    // GPIO pin number associated with this button
    int _pin;
};

#endif
