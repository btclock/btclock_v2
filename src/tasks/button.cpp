/**
 * Button 1: No functionality
 * Button 2: Next Screen
 * Button 3: Previous Screen
 * Button 4: Queue full EPD update
 */

#include "button.hpp"
#ifndef NO_MCP
TaskHandle_t buttonTaskHandle = NULL;
std::vector<EventCallback> buttonEventCallbacks; // Define a vector to hold multiple event callbacks
const TickType_t debounceDelay = pdMS_TO_TICKS(50);
TickType_t lastDebounceTime = 0;

void buttonTask(void *parameter)
{
    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        TickType_t currentTime = xTaskGetTickCount();
        if ((currentTime - lastDebounceTime) >= debounceDelay)
        {
            lastDebounceTime = currentTime;

            if (!digitalRead(MCP_INT_PIN))
            {
                uint pin = mcp.getLastInterruptPin();

                switch (pin)
                {
                case 3:
                    toggleScreenTimer();
                    break;
                case 2:
                    nextScreen();
                    break;
                case 1:
                    previousScreen();
                    break;
                case 0:
                    showNetworkSettings();
                    break;
                }
            }
            mcp.clearInterrupts();
            // Very ugly, but for some reason this is necessary
            while (!digitalRead(MCP_INT_PIN))
            {
                mcp.clearInterrupts();
            }
        }
    }
}

void IRAM_ATTR handleButtonInterrupt()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(buttonTaskHandle, 0, eNoAction, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
}

void setupButtonTask()
{
    xTaskCreate(buttonTask, "ButtonTask", 4096, NULL, 1, &buttonTaskHandle); // Create the FreeRTOS task
    // Use interrupt instead of task
    attachInterrupt(MCP_INT_PIN, handleButtonInterrupt, CHANGE);
}

void registerNewButtonCallback(const EventCallback cb)
{
    buttonEventCallbacks.push_back(cb);
}
#endif