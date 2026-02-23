#ifndef LED_H
#define LED_H

// Led class: controls an LED connected to a digital pin
class Led {
public:
    // Constructor: initializes the Led object with a pin number
    Led(int pin);
    
    // Initializes the LED pin as OUTPUT
    void Init();
    
    // Turns the LED ON
    void On();
    
    // Turns the LED OFF
    void Off();
    
private:
    int _pin;  // Stores the pin number where the LED is connected
};

#endif
