#include <Arduino.h>
#include <stdio.h>
#include "Led.h"
#include "Button.h"
#include "map.h"

// Pin definitions for all hardware peripherals
#define BUTTON_PIN 7
#define GREEN_LED_PIN 13
#define RED_LED_PIN 12
#define YELLOW_LED_PIN 11

// Hardware peripheral objects
Led green(GREEN_LED_PIN);
Led red(RED_LED_PIN);
Led yellow(YELLOW_LED_PIN);
Button btn(BUTTON_PIN);

// Press counters
unsigned long totalPresses = 0;
unsigned long shortPressCount = 0;
unsigned long longPressCount = 0;

// Accumulated durations for average calculation
unsigned long shortDurationSum = 0;
unsigned long longDurationSum = 0;

// Tracks the last time a report was printed
unsigned long lastReportTime = 0;
// Report is printed every 10 seconds
const unsigned long REPORT_INTERVAL = 10000;

// Initializes all peripherals and the map layer
void setup() {

    Serial.begin(9600);   // IMPORTANT for Wokwi Serial Monitor

    green.Init();
    red.Init();
    yellow.Init();
    btn.Init();

    MAP_Init(&green, &red, &yellow, &btn);

    Serial.println("System Started");
}

// Blinks the yellow LED a given number of times with 50ms on/off intervals
void YellowBlink(int times) {

    for (int i = 0; i < times; i++) {
        printf("YELLOW_ON\n");
        delay(50);
        printf("YELLOW_OFF\n");
        delay(50);
    }
}

void loop() {

    unsigned long currentTime = millis();

    // ---------- TIMER ----------
    // Print a statistics report every REPORT_INTERVAL milliseconds
    if (currentTime - lastReportTime >= REPORT_INTERVAL) {

        printf("\n----- 10s REPORT -----\n");
        printf("Total presses: %lu\n", totalPresses);
        printf("Short presses: %lu\n", shortPressCount);
        printf("Long presses: %lu\n", longPressCount);

        // Calculate and print average press duration if any presses occurred
        if (totalPresses > 0) {
            unsigned long totalDuration =
                shortDurationSum + longDurationSum;

            unsigned long average =
                totalDuration / totalPresses;

            printf("Average duration: %lu ms\n", average);
        }
        else {
            printf("Average duration: 0 ms\n");
        }

        printf("----------------------\n");

        // Reset all counters and sums for the next interval
        totalPresses = 0;
        shortPressCount = 0;
        longPressCount = 0;
        shortDurationSum = 0;
        longDurationSum = 0;

        lastReportTime = currentTime;
    }

    // ---------- INPUT ----------
    char buffer[20];

    // Read button press duration from the map layer
    int result = Scanf(buffer, sizeof(buffer));

    if (result > 0) {

        unsigned long duration = atol(buffer);

        // Short press: duration between 1ms and 499ms
        // Flash green LED and blink yellow 5 times
        if (duration < 500 && duration > 0) {
            totalPresses++;
            printf("GREEN_ON\n");
            delay(300);
            printf("GREEN_OFF\n");
            YellowBlink(5);
            shortPressCount++;
            shortDurationSum += duration;
        }
        // Long press: duration 500ms or more
        // Flash red LED and blink yellow 10 times
        else if (duration >= 500) {
            totalPresses++;
            printf("RED_ON\n");
            delay(300);
            printf("RED_OFF\n");
            YellowBlink(10);
            longPressCount++;
            longDurationSum += duration;
        }
    }
}

