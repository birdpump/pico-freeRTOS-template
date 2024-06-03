#include <iostream>
#include <sstream>
#include <cmath>
#include <cstdio>

#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"

#include "Adafruit_NeoPixel.hpp"
#include "queue.h"

#define PIN 27
#define NUMPIXELS 60

using namespace std;


Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

QueueHandle_t xStatusQueue;


void led_task(void *pvParameters) {
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        gpio_put(LED_PIN, 1);
        vTaskDelay(100);
        gpio_put(LED_PIN, 0);
        vTaskDelay(100);
    }
}

void neopixel_task(void *pvParameters) {
    int receivedStatus;

    int g = 0;
    int r = 0;
    int b = 0;

    int s = 0; //speed of fade
    int n = 0; //number of flashes per cycle
    bool on = true;

    while (1) {
        xQueueReceive(xStatusQueue, &receivedStatus, 0);

        if(receivedStatus == 1){
            r = 255;
            g = 136;
            b = 0;

            s = 2;
            n = 2;
            on = false;
        }else if(receivedStatus == 2){
            r = 0;
            g = 255;
            b = 0;

            s = 2;
            n = 2;
            on = false;
        }else if(receivedStatus == 3){
            r = 0;
            g = 0;
            b = 255;

            s = 25;
            n = 1;
            on = true;
        }

        pixels.setPixelColor(0, pixels.Color(g, r, b));

        for (int t = n; t > 0; t--) {
            for (int i = 150; i <= 250; i++) {
                int value = pow(2, i / 32.0);
                pixels.setBrightness(value);
                pixels.show();
                vTaskDelay(pdMS_TO_TICKS(s));
            }

            for (int i = 250; i >= 150; i--) {
                int value = pow(2, i / 32.0);
                pixels.setBrightness(value);
                pixels.show();
                vTaskDelay(pdMS_TO_TICKS(s));
            }
        }

        if (!on) {
            pixels.clear();
            pixels.show();
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


void vSetStatusTask(void *pvParameters) {
    int status = 3;

    while (1) {
        xQueueSend(xStatusQueue, &status, portMAX_DELAY);

        if(status == 3){
            status = 1;
        }else{
            status = 3;
        }

        vTaskDelay(pdMS_TO_TICKS(30000));
    }
}

void setup() {
    pixels.begin();

    xStatusQueue = xQueueCreate(10, sizeof(int));
}

int main() {
    stdio_init_all();
    setup();

    xTaskCreate(led_task, "led_task", 256, NULL, 1, NULL);

    xTaskCreate(vSetStatusTask, "vSetStatusTask", 256, NULL, 2, NULL);

    xTaskCreate(neopixel_task, "neopixel_task", 256, NULL, 3, NULL);

    vTaskStartScheduler();
}