/**
 * Button 1: No functionality
 * Button 2: Next Screen
 * Button 3: Previous Screen
 * Button 4: Queue full EPD update
*/

#include "button.hpp"
#ifndef NO_MCP
TaskHandle_t buttonTaskHandle = NULL;
// Define a type for the event callback
std::vector<EventCallback> buttonEventCallbacks; // Define a vector to hold multiple event callbacks
volatile boolean buttonPressed = false;

void buttonTask(void *parameter)
{
    while (1)
    {
        if (!digitalRead(MCP_INT_PIN))
        {
            uint pin = mcp.getLastInterruptPin();
            if (pin == 3) {
              //  xTaskCreate(fullRefresh, "FullRefresh", 2048, NULL, 1, NULL); 
              toggleScreenTimer();
            }
            else if (pin == 1)
            {
                previousScreen();
            }
            else if (pin == 2)
            {
                nextScreen();
            }
            else if (pin == 0)
            {
                showNetworkSettings();
            }

            vTaskDelay(250); // debounce
            mcp.clearInterrupts(); // clear
        }

        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

void IRAM_ATTR handleButtonInterrupt()
{
    buttonPressed = true;
    // Serial.println(F("ISR"));
    // uint pin = mcp.getLastInterruptPin();

    // if (pin == 1)
    // {
    //     nextScreen();
    // }
    // else if (pin == 2)
    // {
    //     previousScreen();
    // }
    // vTaskDelay(pdMS_TO_TICKS(250));

    // mcp.clearInterrupts();
}

void setupButtonTask()
{
    xTaskCreate(buttonTask, "ButtonTask", 4096, NULL, 1, &buttonTaskHandle); // Create the FreeRTOS task
    // Use interrupt instead of task
    // attachInterrupt(MCP_INT_PIN, handleButtonInterrupt, FALLING);
}

void registerNewButtonCallback(const EventCallback cb)
{
    buttonEventCallbacks.push_back(cb);
}
#endif