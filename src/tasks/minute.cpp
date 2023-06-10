#include "minute.hpp"

TaskHandle_t minuteTaskHandle = NULL;
 // Define a type for the event callback
std::vector<EventCallback> minuteEventCallbacks; // Define a vector to hold multiple event callbacks
bool eventTriggered = false; // Initialize the event triggered flag to false

void minuteTask(void * parameter) {
  while(1) {
    #ifdef IS_3C // wait 5 minutes in case of a 3 color screen otherwise it keeps refreshing
    if(rtc.getMinute() % 5 == 0 && !eventTriggered) {
      eventTriggered = true;
      for(auto &callback : minuteEventCallbacks) { // Loop through all the event callbacks and call them
        callback();
      }
    }
    if(rtc.getMinute() % 5 != 0 && eventTriggered) { // Reset the event triggered flag if the second is not 0
      eventTriggered = false;
    }
    vTaskDelay(pdMS_TO_TICKS(1000)); // Sleep for 1000 milliseconds to avoid busy waiting
    #else
    if(rtc.getSecond() == 0 && !eventTriggered) {
      eventTriggered = true;
      for(auto &callback : minuteEventCallbacks) { // Loop through all the event callbacks and call them
        callback();
      }
    }
    if(rtc.getSecond() != 0) { // Reset the event triggered flag if the second is not 0
      eventTriggered = false;
    }
    vTaskDelay(pdMS_TO_TICKS(500)); // Sleep for 500 milliseconds to avoid busy waiting
    #endif
  }
}

void setupMinuteEvent()
{
    xTaskCreate(minuteTask, "MinuteTask", 2048, NULL, 1, &minuteTaskHandle); // Create the FreeRTOS task
}

void registerNewMinuteCallback(EventCallback cb)
{
  minuteEventCallbacks.push_back(cb);
} 