// FreeRTOS
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <semphr.h>
// CXX
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <algorithm>
// Pico SDK
#include "pico/stdlib.h" // Includes `hardware_gpio.h`
#include "pico/binary_info.h"
#include "pico/cyw43_arch.h"
// App
#include "../Common/utils.h"

#define DEBUG
using std::string;
using std::stringstream;

/**
 * CONSTANTS
 */
#define         LED_FLASH_PERIOD_MS         2000
#define         LED_OFF_PERIOD_MS           100

#define         TIMER_ID_LED_ON             0
#define         TIMER_ID_LED_OFF            255

#define         LED_ON                      1
#define         LED_OFF                     0
#define         LED_ERROR_FLASHES           5


/**
 * PROTOTYPES
 */
void led_on();
void led_off();
void task_led_pico(void* unused_arg);
void task2(void* unused_arg);
void timer_fired_callback(TimerHandle_t timer);


/*
 * GLOBALS
 */

// Task handles
TaskHandle_t handle_task_pico = NULL;
TaskHandle_t handle_task2 = NULL;

// Misc
volatile TimerHandle_t led_on_timer;


/**
 * @brief Turn the on-board LED on.
 */
void led_on() {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
}


/**
 * @brief Turn the on-board LED off.
 */
void led_off() {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
}

/*
 * TASKS
 */

/**
 * @brief Repeatedly flash the Pico's built-in LED.
 */
void task_led_pico(void* unused_arg) {

    // Create a repeating (parameter 3) timer
    led_on_timer = xTimerCreate("LED_ON_TIMER",
                                pdMS_TO_TICKS(LED_FLASH_PERIOD_MS),
                                pdTRUE,
                                (void*)TIMER_ID_LED_ON,
                                timer_fired_callback);
    
    // Start the repeating timer
    if (led_on_timer != NULL) xTimerStart(led_on_timer, 0);
    
    // Start the task loop
    while (true) {
        // NOP
    }
}

void task2(void* unused_arg){

    Utils::log_debug(string("Task2 Starting !!\n"));

    // Start the task loop
    while (true) {
        //Utils::log_debug(string("Task2 Running !!\n"));
    }

}


/**
 * @brief Callback actioned when the post IRQ timer fires.
 *
 * @param timer: The triggering timer.
 */
void timer_fired_callback(TimerHandle_t timer) {

#ifdef DEBUG
    // Report the timer that fired
    uint32_t timer_id = (uint32_t)pvTimerGetTimerID(timer);
    stringstream log_stream;
    log_stream << "Timer fired. ID: " << timer_id << ", LED: " << (timer_id == TIMER_ID_LED_ON ? "on" : "off");
    Utils::log_debug(log_stream.str());
#endif
    
    if (timer == led_on_timer) {
        // The LED ON timer fired so turn the LED on briefly
        led_on();
        
        // Create and start a one-shot timer to turn the LED off
        TimerHandle_t led_off_timer = xTimerCreate("LED_OFF_TIMER",
                                                   pdMS_TO_TICKS(LED_OFF_PERIOD_MS),
                                                   pdFALSE,
                                                   (void*)TIMER_ID_LED_OFF,
                                                   timer_fired_callback);
        
        // Start the one-shot timer
        if (led_off_timer != NULL) xTimerStart(led_off_timer, 0);
    } else {
        // The LED OFF timer fired so turn the LED off
        led_off();
        
        // FROM 1.4.1 -- Kill the timer to prevent OOM panics
        xTimerDelete(timer, 0);
    }
}


/*
 * RUNTIME START
 */

int main() {

    // DEBUG
#ifdef DEBUG
    stdio_init_all();
    // observation cyw43_arch_init should be called after stdio_init_all else it does not work.
    if (cyw43_arch_init()) {
        Utils::log_debug(string("Init Failed"));
        sleep_ms(2000);
    }
    // Pause to allow the USB path to initialize
    sleep_ms(2000);

    // Log app info
    Utils::log_device_info();
#endif
    
    // Set up two tasks
    BaseType_t status_task_pico = xTaskCreate(task_led_pico, "PICO_LED_TASK",  128, NULL, 1, &handle_task_pico);
    BaseType_t status_task2 = xTaskCreate(task2, "TASK2",  128, NULL, 1, &handle_task2);
    
    // Start the FreeRTOS scheduler if any of the tasks are good
    if (status_task_pico == pdPASS || status_task2 == pdPASS) {
        Utils::log_debug(string("Starting Schedular"));
        // Start the scheduler
        vTaskStartScheduler();
    } else {
        // Flash board LED 5 times
        uint8_t count = LED_ERROR_FLASHES;
        while (count > 0) {
            led_on();
            vTaskDelay(100);
            led_off();
            vTaskDelay(100);
            count--;
        }
    }

    // We should never get here, but just in case...
    while(true) {
        // NOP
    };
}
