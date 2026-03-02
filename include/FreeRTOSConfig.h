#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <Arduino.h>

#define configUSE_PREEMPTION            1
#define configUSE_IDLE_HOOK             0
#define configUSE_TICK_HOOK             0
#define configCPU_CLOCK_HZ              ( F_CPU )
#define configTICK_RATE_HZ              ( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES            4
#define configMINIMAL_STACK_SIZE        500
#define configTOTAL_HEAP_SIZE           ( 4500 )
#define configMAX_TASK_NAME_LEN         8
#define configUSE_TRACE_FACILITY        0
#define configUSE_16_BIT_TICKS          1
#define configIDLE_SHOULD_YIELD         1

#define configUSE_MUTEXES               1
#define configQUEUE_REGISTRY_SIZE       0
#define configCHECK_FOR_STACK_OVERFLOW  2
#define configUSE_MALLOC_FAILED_HOOK    1

#define configSUPPORT_DYNAMIC_ALLOCATION 1
#define configSUPPORT_STATIC_ALLOCATION  0

#endif