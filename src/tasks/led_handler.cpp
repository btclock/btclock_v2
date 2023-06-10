#include "led_handler.hpp"

#ifdef WITH_RGB_LED
TaskHandle_t ledHandlerTaskHandle = NULL;

void ledHandlerTask(void *parameter)
{
    int dir = 5;
    int bright = 0;
    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        for (int i = 0; i < 3; i++) {
        pixels.fill(pixels.Color(224, 67, 0));                                   
        pixels.show();  
        vTaskDelay(pdMS_TO_TICKS(600));      
        pixels.fill(pixels.Color(8, 2, 0));                                   
        pixels.show();  
        vTaskDelay(pdMS_TO_TICKS(400));    
        }

        pixels.fill(pixels.Color(0,0,0));
        pixels.show(); 
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void setupLedHandlerTask()
{
    xTaskCreate(ledHandlerTask, "LedHandlerTask", 2048, NULL, 1, &ledHandlerTaskHandle); // Create the FreeRTOS task
}
#endif