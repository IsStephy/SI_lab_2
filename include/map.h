#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include "Led.h"
#include "Button.h"

// Initializes the hardware abstraction layer with the provided peripherals
// green  - pointer to the green LED object
// red    - pointer to the red LED object
// yellow - pointer to the yellow LED object
// btn    - pointer to the button object
void MAP_Init(Led* green, Led* red, Led* yellow, Button* btn);

// Reads a string input into buffer up to maxLen characters
// Returns the number of characters read
int Scanf(char* buffer, int maxLen);

#endif