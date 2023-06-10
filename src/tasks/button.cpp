#include "button.hpp"
#ifndef NO_MCP
TaskHandle_t buttonTaskHandle = NULL;
// Define a type for the event callback
std::vector<EventCallback> buttonEventCallbacks; // Define a vector to hold multiple event callbacks


void buttonTask(void *parameter)
{
    while (1)
    {
        for (int i = 0; i < 4; i++)
        {
            if (!mcp.digitalRead(i))
            {
                Serial.println("Button " + String(i) + " Pressed!");
                delay(250);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

void setupButtonTask()
{
    xTaskCreate(buttonTask, "MinuteTask", 2048, NULL, 1, &buttonTaskHandle); // Create the FreeRTOS task
}

void registerNewButtonCallback(EventCallback cb)
{
    buttonEventCallbacks.push_back(cb);
}
#endif