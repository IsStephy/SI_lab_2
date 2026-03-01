#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio.h>
#include "Led.h"
#include "Button.h"
#include "map.h"

/* =======================
   HARDWARE OBJECTS
   ======================= */

Button button(7);
Led greenLed(13);
Led redLed(12);
Led yellowLed(11);

/* =======================
   GLOBAL SHARED VARIABLES
   ======================= */

volatile uint16_t totalPresses = 0;
volatile uint16_t shortPresses = 0;
volatile uint16_t longPresses = 0;
volatile uint16_t totalShortDuration = 0;
volatile uint16_t totalLongDuration = 0;
volatile uint16_t lastPressDuration = 0;

/* =======================
   SYNCHRONIZATION OBJECTS
   ======================= */

SemaphoreHandle_t xButtonSemaphore;
SemaphoreHandle_t xStatsMutex;

/* =======================
   TASK 1 – BUTTON
   ======================= */

void Task_Button(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(10);
    

    bool lastState = false;
    uint16_t pressStart = 0;

    for(;;)
    {
        bool currentState = button.IsPressed();

        if(currentState && !lastState)
        {
            pressStart = millis();
            vTaskDelay(pdMS_TO_TICKS(20));
        }

        if(!currentState && lastState)
        {
            vTaskDelay(pdMS_TO_TICKS(20));

            uint16_t duration = millis() - pressStart;

            xSemaphoreTake(xStatsMutex, portMAX_DELAY);
            lastPressDuration = duration;
            xSemaphoreGive(xStatsMutex);

            if(duration < 500)
            {
                printf("g_on\n");
                printf("r_off\n");
            }
            else
            {
                printf("g_off\n");
                printf("r_on\n");
            }

            xSemaphoreGive(xButtonSemaphore);  // Give semaphore AFTER button release
        }

        lastState = currentState;
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

/* =======================
   TASK 2 – STATISTICS
   ======================= */

void Task_Statistics(void *pvParameters)
{
    
    
    for(;;)
    {
        // Wait for button press signal
        xSemaphoreTake(xButtonSemaphore, portMAX_DELAY);

        xSemaphoreTake(xStatsMutex, portMAX_DELAY);

        totalPresses++;

        if(lastPressDuration < 500)
        {
            shortPresses++;
            totalShortDuration += lastPressDuration;
        }
        else
        {
            longPresses++;
            totalLongDuration += lastPressDuration;
        }

        uint8_t blinks;
        if (totalLongDuration % 5 == 0){
            blinks = 5;
        }else{
            blinks = 10;
        }

        xSemaphoreGive(xStatsMutex);

        for(uint8_t i = 0; i < blinks; i++)
        {
            printf("y_on\n");
            vTaskDelay(pdMS_TO_TICKS(50));
            printf("y_off\n");
            vTaskDelay(pdMS_TO_TICKS(50));
        }

        printf("g_off\n");
        printf("r_off\n");
    }
}

/* =======================
   TASK 3 – REPORT
   ======================= */

void Task_Report(void *pvParameters)
{
    
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(10000);

    for(;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xPeriod);

        xSemaphoreTake(xStatsMutex, portMAX_DELAY);

        uint16_t avg = 0;
        if(totalPresses > 0)
            avg = (totalShortDuration + totalLongDuration) / totalPresses;

        printf("T:%u S:%u L:%u A:%u\n",
               totalPresses,
               shortPresses,
               longPresses,
               avg);

        totalPresses = 0;
        shortPresses = 0;
        longPresses = 0;
        totalShortDuration = 0;
        totalLongDuration = 0;

        xSemaphoreGive(xStatsMutex);
    }
}

/* =======================
   SETUP
   ======================= */

void setup()
{
    Serial.begin(9600);


    button.Init();
    greenLed.Init();
    redLed.Init();
    yellowLed.Init();

    MAP_Init(&greenLed, &redLed, &yellowLed, &button);

    xButtonSemaphore = xSemaphoreCreateBinary();
    
    xStatsMutex = xSemaphoreCreateMutex();

    xTaskCreate(Task_Button, "B", 80, NULL, 1, NULL);
    xTaskCreate(Task_Statistics, "S", 80, NULL, 1, NULL);
    xTaskCreate(Task_Report, "R", 120, NULL, 3, NULL);  // Highest priority

    vTaskStartScheduler();
}

void loop() {}