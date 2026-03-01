#include "map.h"
#include <Arduino.h>
#include <string.h>

// Pointers to the hardware peripherals used by the map layer
static Led* greenLed;
static Led* redLed;
static Led* yellowLed;
static Button* button;

// Custom FILE stream used to redirect printf to the map layer
FILE map_stdout;

/* -------- printf mapping -------- */

// Custom putchar function that intercepts printf output
// Buffers characters until a newline, then checks if the accumulated
// string is a known LED command and executes it, otherwise prints to Serial
int map_putchar(char c, FILE* f) {
    static char command[32];   // reduced buffer (UNO friendly)
    static uint8_t idx = 0;

    if (c == '\n') {
        command[idx] = '\0';

        if (strcmp(command, "g_on") == 0)
            greenLed->On();

        else if (strcmp(command, "g_off") == 0)
            greenLed->Off();

        else if (strcmp(command, "r_on") == 0)
            redLed->On();

        else if (strcmp(command, "r_off") == 0)
            redLed->Off();

        else if (strcmp(command, "y_on") == 0)
            yellowLed->On();

        else if (strcmp(command, "y_off") == 0)
            yellowLed->Off();

        else {
            if (idx > 0) {
                Serial.println(command);
            }
        }

        idx = 0;
    }
    else {
        if (idx < sizeof(command) - 1)
            command[idx++] = c;
    }

    return 0;
}

/* -------- scanf mapping -------- */

// Reads button press duration and writes it as a string into buffer
// Measures time from press to release in milliseconds
// Returns the number of characters written, or 0 if no complete press detected
int Scanf(char* buffer, int maxLen) {

    static bool wasPressed = false;
    static unsigned long pressStartTime = 0;

    // Check if button is currently pressed
    bool isPressed = button->IsPressed();

    // Transition from not-pressed to pressed: record the start time
    if (isPressed && !wasPressed) {
        pressStartTime = millis();
        wasPressed = true;
        return 0;  // Still waiting for release
    }

    // Button is released after being pressed: calculate and store duration
    if (!isPressed && wasPressed) {
        wasPressed = false;
        unsigned long duration = millis() - pressStartTime;
        snprintf(buffer, maxLen, "%lu", duration);
        return strlen(buffer);
    }

    // No state change
    return 0;
}

/* -------- init -------- */

// Initializes the map layer by storing peripheral pointers and
// redirecting stdout to the custom map_putchar stream
void MAP_Init(Led* green, Led* red, Led* yellow, Button* btn) {

    greenLed = green;
    redLed = red;
    yellowLed = yellow;
    button = btn;

    // Set up the custom output stream and redirect stdout to it
    fdev_setup_stream(&map_stdout,
                      map_putchar,
                      NULL,
                      _FDEV_SETUP_WRITE);

    stdout = &map_stdout;
}
